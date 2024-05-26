#include "pch.h"

#include "Core/FileDialog.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <utility>

std::string FileDialog::Show() {
  char buffer[128];
  std::string command = "zenity --file-selection --title=\"Select a file\"";
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe)
    return "Error opening pipe";
  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != NULL) {
      buffer[strcspn(buffer, "\n")] = 0;  // Strip newline character
      return buffer;
    }
  }
  pclose(pipe);
  return "";
}

std::string FileDialog::GetFilter() {
  std::string filter;
  for (auto &[description, extension] : m_Extensions) {
    filter += " --file-filter=\"" + description + " | *." + extension + "\"";
  }
  return filter;
}