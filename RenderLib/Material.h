#include "core.h"

#include "Shader.h"
#include "Texture.h"
#include <unordered_map>

class UniformBase {
protected:
    std::string name;
    ptr<Shader> shader;

public:
    UniformBase(ptr<Shader> shader, const std::string& name) : name(name), shader(shader) {}
    virtual void SetValue(const void* value) = 0;
    virtual void Apply() const = 0;
};

template <typename T>
class Uniform : public UniformBase {
    T value;

public:
    Uniform(ptr<Shader> shader, const std::string& name, const T& value) 
        : value(value), UniformBase(shader, name) {}

    void SetValue(const void* value) override
    {
        // weird casting
        this->value = *static_cast<T*>(const_cast<void*>(value));
    }

    void Apply() const override {
        shader->SetUniform(name, value);
    }
};

class Material {
    std::string name;
    ptr<Shader> shader;
    ptr<Texture> texture;
    std::unordered_map<std::string, std::unique_ptr<UniformBase>> uniforms;

public:
    Material(const std::string& name, ptr<Shader> shader, ptr<Texture> tex = std::make_shared<Texture>()) : name(name), shader(shader), texture(tex) {}

    template <typename T>
    void SetUniform(const std::string& name, const T& value) {
        if (uniforms.find(name) == uniforms.end())
            uniforms[name] = std::make_unique<Uniform<T>>(shader, name, value);
        uniforms[name]->SetValue(&value);
    }

    void SetTexture(ptr<Texture> tex) {
        texture = tex;
    }

    void Bind();
};