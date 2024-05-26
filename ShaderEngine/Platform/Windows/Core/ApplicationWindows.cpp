#include "Core/Application.h"
#include "pch.h"

std::string Application::GetApplicationPath() {
  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  std::string fullPath(buffer);
  std::filesystem::path path(fullPath);
  std::string directory = path.parent_path().string();

  return directory;
}