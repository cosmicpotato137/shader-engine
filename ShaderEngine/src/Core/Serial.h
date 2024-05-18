#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <type_traits>

#include "imgui/imgui.h"
#include "Core/core.h"
#include "Core/Log.h"

// Helper defines for shader engine serialization
#define SE_SERIAL_FRIENDS                    \
  friend class boost::serialization::access; \
  friend class Serial

#define SE_SERIAL_VERSION(T, version) BOOST_CLASS_VERSION(T, version)

// Helper class for boost serialization
class Serial {
public:
  // Save object to file
  template <typename T>
  static void Save(const T &serializable, const std::string &filepath);

  // Load new object from file
  template <typename T> static T Load(const std::string &filepath);

  // Load object values into existing object
  template <typename T>
  static void LoadInplace(T &obj, const std::string &filepath);
};

template <typename T>
void Serial::Save(const T &serializable, const std::string &filepath) {
  // Check if we can save to this path/file
  std::filesystem::path path(filepath);
  if (!std::filesystem::exists(path.parent_path())) {
    Console::Error("Path does not exist: %s", path.parent_path().string());
    return;
  }

  std::ofstream ofs(filepath);
  boost::archive::text_oarchive oa(ofs);

  // Check if object implements serialize method
  try {
    oa << serializable;
  } catch (const boost::archive::archive_exception &e) {
    Console::Error("Failed to serialize object: %s", e.what());
    return;
  }
}

template <typename T> T Serial::Load(const std::string &filepath) {
  // Check if file exists
  if (!std::filesystem::exists(filepath)) {
    Console::Error("File does not exist: %s", filepath);
    return T();
  }

  T obj;
  std::ifstream ifs(filepath);
  boost::archive::text_iarchive ia(ifs);

  try {
    ia >> obj;
  } catch (const boost::archive::archive_exception &e) {
    Console::Error("Failed to deserialize object: %s", e.what());
    return T();
  }

  return obj;
}

template <typename T>
void Serial::LoadInplace(T &obj, const std::string &filepath) {
  // Check if file exists
  if (!std::filesystem::exists(filepath)) {
    Console::Error("File does not exist: %s", filepath);
    return;
  }

  std::ifstream ifs(filepath);
  boost::archive::text_iarchive ia(ifs);

  try {
    ia >> obj;
  } catch (const boost::archive::archive_exception &e) {
    Console::Error("Failed to deserialize object: %s", e.what());
  }
}

namespace boost {
namespace serialization {

// Shared pointer serialization
template <class Archive, class T>
void serialize(Archive &ar, ptr<T> &p, const unsigned int version) {
  // if loading and p is nullptr, create new object
  if (Archive::is_loading::value && p == nullptr)
    p = std::make_shared<T>();
  ar &*p;
}

// Vec2 serialization
template <class Archive>
void serialize(Archive &ar, glm::vec2 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
}

// Vec3 serialization
template <class Archive>
void serialize(Archive &ar, glm::vec3 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
  ar & v.z;
}

// Vec4 serialization
template <class Archive>
void serialize(Archive &ar, glm::vec4 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
  ar & v.z;
  ar & v.w;
}

// Mat4 serialization
template <class Archive>
void serialize(Archive &ar, glm::mat4 &m, const unsigned int version) {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      ar &m[i][j];
}

// ImVec2 serialization
template <class Archive>
void serialize(Archive &ar, ImVec2 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
}

}  // namespace serialization
}  // namespace boost
