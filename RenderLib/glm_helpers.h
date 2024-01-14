#pragma once
#include "glm/glm.hpp"
#include <iostream>

// For glm::vec2
std::ostream &operator<<(std::ostream &os, const glm::vec2 &v) {
  os << v.x << ' ' << v.y;
  return os;
}

std::istream &operator>>(std::istream &is, glm::vec2 &v) {
  is >> v.x >> v.y;
  return is;
}

// For glm::vec3
#include <glm/glm.hpp>  // Include the necessary header file for the glm library

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
  os << v.x << ' ' << v.y << ' ' << v.z;
  return os;
}

std::istream &operator>>(std::istream &is, glm::vec3 &v) {
  is >> v.x >> v.y >> v.z;
  return is;
}

// For glm::vec4
std::ostream &operator<<(std::ostream &os, const glm::vec4 &v) {
  os << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
  return os;
}

std::istream &operator>>(std::istream &is, glm::vec4 &v) {
  is >> v.x >> v.y >> v.z >> v.w;
  return is;
}

// For glm::mat4
std::ostream &operator<<(std::ostream &os, const glm::mat4 &m) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      os << m[i][j] << ' ';
    }
    os << '\n';
  }
  return os;
}

std::istream &operator>>(std::istream &is, glm::mat4 &m) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      is >> m[i][j];
    }
  }
  return is;
}