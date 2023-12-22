#pragma once 

#include "core.h"
#include <variant>
#include <map>

typedef std::variant<bool, GLint, GLuint, GLfloat, glm::vec2,
    glm::vec3, glm::vec4, glm::mat4> uniform_types;

enum UniformType {
    Error = -1, Bool = 0, Float, Int, UInt, Vec2, Vec3, Col3, Vec4, Col4, Mat4, Texture2D, Image2D
};

class Uniform {
protected:
    std::string name;
    GLuint location;
    UniformType type;
    bool hide;

    uniform_types value;

public:
    Uniform(const std::string& name, int location, uniform_types value, UniformType type=UniformType::Error, bool hide = false) 
        : name(name), location(location), value(value), type(type), hide(hide) {}

    bool GetHide() const { return hide; }
    std::string GetName() const { return name; }
    int GetLocation() const { return location; }
    UniformType GetType() const { return type; }

    void SetValue(const uniform_types& value) {
        this->value = value;
    }

    template <typename T>
    T GetValue() const {
        return std::get<T>(this->value);
    }

    uniform_types GetValue() const {
        return this->value;
    }
};

ptr<Uniform> glToShaderUniform(const char* name, int location, GLuint type, GLsizei size);

class Shader {
protected:
    GLuint program;
    std::string filepath;

    std::map<std::string, GLuint> uniformLocations;

public:
    std::string name;
    std::map<std::string, ptr<Uniform>> uniforms;

    Shader(const std::string& name) : program(0), name(name), filepath("") {}
    ~Shader() { Cleanup(); }

    bool Init(const std::string & shaderPath);
    virtual bool InitFromSource(const std::string & shaderSource);

    // Use the shader program
    virtual void Use();
    void Drop() const { glUseProgram(0); }

    void Cleanup();

    // find the location of a uniform
    GLint GetUniformLocation(const std::string& name);

    GLuint GetProgramID() const { return program; }
    std::string GetFilePath() const { return filepath; }
    
    void SetUniform(const std::string& uniform_name, const uniform_types& value);
    bool HasUniform(const std::string& uniformName) const { return uniforms.find(uniformName) != uniforms.end(); }
    std::map<std::string, ptr<Uniform>> GetUniforms();

protected:
    bool LoadSource(const std::string& filepath, std::string& shaderSource);
    
    GLuint Compile(GLenum shaderType, const char* shaderSource);

    GLuint Link(std::vector<GLuint> programs);

    void FindUniforms();
    void ApplyUniforms();

private:
    void ParseVertexAndFragment(const std::string& input, std::string& vertexShader, std::string& fragmentShader);
};