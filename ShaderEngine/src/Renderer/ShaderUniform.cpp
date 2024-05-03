#include "ShaderUniform.h"
#include "Core/Log.h"
#include "Math/glm_helpers.h"

#include <regex>

std::ostream &operator<<(std::ostream &os, const Uniform &uniform) {
  // Write object data
  os << uniform.name << '\n';
  os << static_cast<int>(uniform.type) << '\n';
  os << uniform.hide << '\n';

  // Serialize the `value` member based on the type of value it holds
  std::visit([&os](auto &&arg) { os << arg << '\n'; }, uniform.value);
  return os;
}

std::istream &operator>>(std::istream &is, Uniform &uniform) {
  // Read object data
  getline(is, uniform.name);
  int type;
  is >> type;
  uniform.type = static_cast<UniformType>(type);
  is >> uniform.hide;
  switch (uniform.type) {
  case UniformType::Bool:
    {
      bool b;
      is >> b;
      uniform.value = b;
      break;
    }
  case UniformType::Int:
    {
      GLint i;
      is >> i;
      uniform.value = i;
      break;
    }
  case UniformType::Float:
    {
      GLfloat f;
      is >> f;
      uniform.value = f;
      break;
    }
  case UniformType::Vec2:
    {
      glm::vec2 v2;
      is >> v2;
      uniform.value = v2;
      break;
    }
  case UniformType::Vec3:
  case UniformType::Col3:
    {
      glm::vec3 v3;
      is >> v3;
      uniform.value = v3;
      break;
    }
  case UniformType::Vec4:
  case UniformType::Col4:
    {
      glm::vec4 v4;
      is >> v4;
      uniform.value = v4;
      break;
    }
  case UniformType::Mat4:
    {
      glm::mat4 m4;
      is >> m4;
      uniform.value = m4;
      break;
    }
  }
  return is;
}

ptr<Uniform>
glToShaderUniform(const char *name, int location, GLuint type, GLsizei size) {
  // Check inspector hide/show
  std::regex checkColor("^_?((color)|(c_))[a-zA-Z]*$");

  bool hidden = false;
  std::regex checkHide("^_.*$");
  if (std::regex_search(name, checkHide))
    hidden = true;

  switch (type) {
  case GL_BOOL:
    return std::make_shared<Uniform>(
        std::string(name), location, false, Bool, hidden);
  case GL_INT:
    return std::make_shared<Uniform>(
        std::string(name), location, 0, Int, hidden);
  case GL_UNSIGNED_INT:
    return std::make_shared<Uniform>(
        std::string(name), location, (unsigned int)0, UInt, hidden);
  case GL_FLOAT:
    return std::make_shared<Uniform>(
        std::string(name), location, 0.0f, Float, hidden);
  case GL_FLOAT_VEC2:
    return std::make_shared<Uniform>(
        std::string(name), location, glm::vec2(0), Vec2, hidden);
  case GL_FLOAT_VEC3:
    {
      UniformType t = Vec3;
      if (std::regex_search(name, checkColor))
        t = Col3;
      return std::make_shared<Uniform>(
          std::string(name), location, glm::vec3(0), t, hidden);
    }
  case GL_FLOAT_VEC4:
    {
      UniformType t = Vec4;
      if (std::regex_search(name, checkColor))
        t = Col4;
      return std::make_shared<Uniform>(
          std::string(name), location, glm::vec4(0), t, hidden);
    }
  case GL_FLOAT_MAT4:
    return std::make_shared<Uniform>(
        std::string(name), location, glm::mat4(1), Mat4, true);
  case GL_SAMPLER_2D:
    return std::make_shared<Uniform>(
        std::string(name), location, 0, Texture2D, true);
  case GL_IMAGE_2D:
    return std::make_shared<Uniform>(
        std::string(name), location, 0, Image2D, true);
  default:
    Console::Assert(
        false, "GL type %u not found when creating uniform %s", type, name);
    return std::make_shared<Uniform>(
        std::string(name), location, 0, Error, true);
  }
}