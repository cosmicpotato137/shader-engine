#include "Material.h"

void Material::Bind() 
{
    shader->Use();
    texture->Bind();
}