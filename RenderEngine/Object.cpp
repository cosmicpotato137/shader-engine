#pragma once
#include "Object.h"
#include "Log.h"
#include <fstream>
#include <filesystem>

void Object::Save(Object &obj, const std::string &filepath) {
  // get directory from filepath
  std::filesystem::path path(filepath);
  std::string dir = path.parent_path().string();

  // create directory if it doesn't exist
  if (!std::filesystem::exists(dir)) {
    std::filesystem::create_directories(dir);
  }

  std::ofstream file(filepath, std::ios::binary);
  Console::Assert(file.is_open(), "Could not open file: %s", filepath);
  obj.Serialize(file);
  file.close();
}

void Object::Load(Object &obj, const std::string &filepath) {
  std::ifstream file(filepath, std::ios::binary);
  Console::Assert(file.is_open(), "Could not open file: %s", filepath);
  obj.Deserialize(file);
  file.close();
}