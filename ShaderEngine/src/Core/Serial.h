#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/map.hpp>
#include <fstream>
#include <type_traits>

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
  template <typename T>
  static void Save(const T &serializable, const std::string &filepath);

  template <typename T> static auto Load(const std::string &filepath);
  // -> std::enable_if_t<has_serialize<T>::value, std::unique_ptr<T>>
};

template <typename T>
void Serial::Save(const T &serializable, const std::string &filepath) {
  std::ofstream ofs(filepath);
  boost::archive::text_oarchive oa(ofs);

  // Check if object implements serialize method
  oa << serializable;
}

// -> std::enable_if_t<has_serialize<T>::value, std::unique_ptr<T>>
template <typename T> auto Serial::Load(const std::string &filepath) {
  T obj;
  std::ifstream ifs(filepath);
  boost::archive::text_iarchive ia(ifs);

  ia >> obj;

  return std::make_unique<T>(obj);
}

namespace boost {
namespace serialization {

// shared pointer serialization
template <class Archive, class T>
void serialize(Archive &ar, ptr<T> &p, const unsigned int version) {
  T *raw_ptr = p.get();
  ar & raw_ptr;
  if (Archive::is_loading::value) {
    p.reset(raw_ptr);
  }
}

template <class Archive>
void serialize(Archive &ar, glm::vec2 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
}

template <class Archive>
void serialize(Archive &ar, glm::vec3 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
  ar & v.z;
}

template <class Archive>
void serialize(Archive &ar, glm::vec4 &v, const unsigned int version) {
  ar & v.x;
  ar & v.y;
  ar & v.z;
  ar & v.w;
}

template <class Archive>
void serialize(Archive &ar, glm::mat4 &m, const unsigned int version) {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      ar &m[i][j];
}

}  // namespace serialization
}  // namespace boost
