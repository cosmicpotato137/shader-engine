#include "Material.h"

void Material::Bind() 
{
    shader->Use();
    texture->Bind();
    for (const auto& uniform : uniforms) {
        uniform.second->Apply();
    }
}