#pragma once
#include "core.h"

#include "Shader.h"
#include "Texture.h"
#include <unordered_map>

// Todo add uniform support for multiple textures
class Material {
  std::string name;
  ptr<Shader> shader;
  ptr<Texture> texture;

public:
  Material(
      const std::string &name, ptr<Shader> shader,
      ptr<Texture> tex = std::make_shared<Texture>())
      : name(name), shader(shader), texture(tex) {}

  // Set value of shader
  void SetUniform(const std::string &name, const uniform_types &value);

  // Set the texture for shading
  void SetTexture(ptr<Texture> tex);

  // Bind material for rendering
  void Bind();

  // Serialization
  friend std::ostream &operator<<(std::ostream &os, const Material &material);

  // Deserialization
  friend std::istream &operator>>(std::istream &is, Material &material);
};