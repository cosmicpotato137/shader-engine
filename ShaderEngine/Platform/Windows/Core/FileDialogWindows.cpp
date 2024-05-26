#include "pch.h"

#include "Core/FileDialog.h"
#include <windows.h>
#include <commdlg.h>
#include <vector>
#include <utility>

std::string FileDialog::Show() {
  OPENFILENAME ofn;
  char szFile[260];
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(szFile);
  // ofn.lpstrFilter = GetFilter().c_str();
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn) == TRUE) {
    return ofn.lpstrFile;
  }

  return "";
}

std::string FileDialog::GetFilter() {
  std::string filter;
  for (auto &[description, extension] : m_Extensions) {
    filter += description + " (*." + extension + ")";
    filter += '\0';
    filter += "*." + extension;
    filter += '\0';
  }
  filter += '\0';
  return filter;
}