#pragma once
#include "Application.h"

extern Application *CreateApplication(const ApplicationCommandLineArgs &args);

int main(int argc, char **argv) {

  auto *app = CreateApplication({argc, argv});

  if (app->Init()) {
    // Run the application
    app->Run();
    // Clean up all resources
    Application::DestroyInstance();
  } else {
    Console::Assert(false, "Unable to run the app");
  }
}