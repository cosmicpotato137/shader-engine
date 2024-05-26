#include "Core/Application.h"
#include "pch.h"

std::string Application::GetApplicationPath() {
  std::string fullPath = std::filesystem::canonical("/proc/self/exe").string();
  std::filesystem::path path(fullPath);
  std::string directory = path.parent_path().string();

  return directory;
}