#pragma once
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <vector>

class FileDialog {
public:
  FileDialog(const std::vector<std::pair<std::string, std::string>> &extensions)
      : m_Extensions(extensions) {}

  std::string Show();

private:
  std::vector<std::pair<std::string, std::string>> m_Extensions;

  std::string GetFilter();
};