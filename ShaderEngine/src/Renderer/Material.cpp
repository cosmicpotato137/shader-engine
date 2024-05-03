#include "pch.h"
#include "Material.h"

void Material::Cleanup() {
  shader->Cleanup();
  texture->Cleanup();
}

void Material::SetUniform(const std::string &name, const uniform_types &value) {
  shader->SetUniform(name, value);
}

void Material::SetTexture(ptr<Texture> tex) { texture = tex; }

void Material::Bind() {
  shader->Use();
  texture->Bind();
}

std::istream &operator>>(std::istream &is, Material &material) {
  // Read object data
  getline(is, material.name);
  is >> *(material.shader);
  is >> *(material.texture);
  return is;
}

std::ostream &operator<<(std::ostream &os, const Material &material) {
  // Write object data
  os << material.name << '\n';
  os << *(material.shader) << '\n';
  os << *(material.texture) << '\n';
  return os;
}
