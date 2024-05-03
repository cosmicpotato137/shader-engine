#pragma once

#include "Core/core.h"
#include <variant>

typedef std::variant<
    bool, GLint, GLuint, GLfloat, glm::vec2, glm::vec3, glm::vec4, glm::mat4>
    uniform_types;

enum UniformType {
  Error = -1,
  Bool = 0,
  Float,
  Int,
  UInt,
  Vec2,
  Vec3,
  Col3,
  Vec4,
  Col4,
  Mat4,
  Texture2D,
  Image2D
};

class Uniform {
protected:
  std::string name;
  GLuint location;
  UniformType type;
  bool hide;

  uniform_types value;

public:
  Uniform() : name(""), location(-1), type(UniformType::Error), hide(true) {}
  Uniform(
      const std::string &name, int location, uniform_types value,
      UniformType type = UniformType::Error, bool hide = false)
      : name(name), location(location), value(value), type(type), hide(hide) {}

  bool GetHide() const { return hide; }
  std::string GetName() const { return name; }
  int GetLocation() const { return location; }
  UniformType GetType() const { return type; }

  void SetValue(const uniform_types &value) { this->value = value; }

  template <typename T> T GetValue() const { return std::get<T>(this->value); }

  uniform_types GetValue() const { return this->value; }

  // Serialization
  friend std::ostream &operator<<(std::ostream &os, const Uniform &uniform);

  // Deserialization
  friend std::istream &operator>>(std::istream &is, Uniform &uniform);
};

ptr<Uniform>
glToShaderUniform(const char *name, int location, GLuint type, GLsizei size);
