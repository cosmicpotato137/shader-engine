#include "Material.h"

void Material::SetUniform(const std::string &name, const uniform_types &value) {
  shader->SetUniform(name, value);
}

void Material::SetTexture(ptr<Texture> tex) { texture = tex; }

void Material::Bind() {
  shader->Use();
  texture->Bind();
}