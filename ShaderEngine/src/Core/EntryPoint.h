#include <windows.h>
#include <shellapi.h>
#include "Application.h"

extern Application *CreateApplication(const ApplicationCommandLineArgs &args);

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
    int nCmdShow) {
  int argc;
  LPWSTR *argv_wide = CommandLineToArgvW(GetCommandLineW(), &argc);

  if (argv_wide) {
    // Convert LPWSTR* argv to char** argv
    char **argv = new char *[argc];
    for (int i = 0; i < argc; ++i) {
      int wlen = lstrlenW(argv_wide[i]);
      int len = WideCharToMultiByte(
          CP_ACP, 0, argv_wide[i], wlen, NULL, 0, NULL, NULL);
      argv[i] = new char[len + 1];
      WideCharToMultiByte(
          CP_ACP, 0, argv_wide[i], wlen, argv[i], len, NULL, NULL);
      argv[i][len] = '\0';
    }

    auto *app = CreateApplication({argc, argv});

    if (app->Init()) {
      // Run the application
      app->Run();
      // Clean up all resources
      Application::DestroyInstance();
    } else {
      Console::Assert(false, "Unable to run the app");
    }

    // Don't forget to free the memory
    for (int i = 0; i < argc; ++i)
      delete[] argv[i];
    delete[] argv;

    LocalFree(argv_wide);
  }

  return 0;
}