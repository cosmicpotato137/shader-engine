#include "core.h"

#include "Shader.h"
#include "Texture.h"
#include <unordered_map>

class Material {
    std::string name;
    ptr<Shader> shader;
    ptr<Texture> texture;

public:
    Material(const std::string& name, ptr<Shader> shader, ptr<Texture> tex = std::make_shared<Texture>()) : name(name), shader(shader), texture(tex) {}

    void SetUniform(const std::string& name, const uniform_types& value) {
        shader->SetUniform(name, value);
    }

    void SetTexture(ptr<Texture> tex) {
        texture = tex;
    }

    void Bind();
};