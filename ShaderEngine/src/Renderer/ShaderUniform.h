#pragma once

#include "Core/core.h"
#include "Core/Serial.h"
#include <boost/variant/get.hpp>
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

  // Set the location of the uniform in the shader
  // WARNING: Only use this if you know what you are doing
  int SetLocation(int location) { return this->location = location; }

  UniformType GetType() const { return type; }

  void SetValue(const uniform_types &value) { this->value = value; }

  template <typename T> T GetValue() const { return std::get<T>(this->value); }

  uniform_types GetValue() const { return this->value; }

private:
  SE_SERIAL_FRIENDS;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & name;
    ar & type;
    ar & hide;

    switch (type) {
    case UniformType::Bool:
      {
        bool b;
        if (std::holds_alternative<bool>(value))
          b = std::get<bool>(value);
        ar & b;
        value = b;
        break;
      }
    case UniformType::UInt:
      {
        GLuint ui;
        if (std::holds_alternative<GLuint>(value))
          ui = std::get<GLuint>(value);
        ar & ui;
        value = ui;
        break;
      }
    case UniformType::Int:
      {
        GLint i;
        if (std::holds_alternative<GLint>(value))
          i = std::get<GLint>(value);
        ar & i;
        value = i;
        break;
      }
    case UniformType::Float:
      {
        GLfloat f;
        if (std::holds_alternative<GLfloat>(value))
          f = std::get<GLfloat>(value);
        ar & f;
        value = f;
        break;
      }
    case UniformType::Vec2:
      {
        glm::vec2 v2;
        if (std::holds_alternative<glm::vec2>(value))
          v2 = std::get<glm::vec2>(value);
        ar & v2;
        value = v2;
        break;
      }
    case UniformType::Vec3:
    case UniformType::Col3:
      {
        glm::vec3 v3;
        if (std::holds_alternative<glm::vec3>(value))
          v3 = std::get<glm::vec3>(value);
        ar & v3;
        value = v3;
        break;
      }
    case UniformType::Vec4:
    case UniformType::Col4:
      {
        glm::vec4 v4;
        if (std::holds_alternative<glm::vec4>(value))
          v4 = std::get<glm::vec4>(value);
        ar & v4;
        value = v4;
        break;
      }
    case UniformType::Mat4:
      {
        glm::mat4 m4;
        if (std::holds_alternative<glm::mat4>(value))
          m4 = std::get<glm::mat4>(value);
        ar & m4;
        value = m4;
        break;
      }
    case UniformType::Error:
    default:
      {
        // Set to -1
        GLint i = -1;
        ar & i;
        value = i;
        break;
      }
    }
  }
};

ptr<Uniform>
glToShaderUniform(const char *name, int location, GLuint type, GLsizei size);
