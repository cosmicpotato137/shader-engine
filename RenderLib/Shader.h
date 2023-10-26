#pragma once 

#include "core.h"


class Shader {
protected:
    GLuint program;

    std::map<std::string, GLuint> uniformLocations;

public:
    std::string name;

    Shader(const std::string& name) : program(0), name(name) {}
    ~Shader() { Cleanup(); }

    bool Init(const std::string & shaderPath);
    virtual bool InitFromSource(const std::string & shaderSource);

    // Use the shader program
    virtual void Use() const { glUseProgram(program); }
    void Drop() const { glUseProgram(0); }

    void Cleanup();

    // find the location of a uniform
    GLint GetUniformLocation(const std::string& name);
    
    // Set a uniform integer value
    bool SetUniform(const std::string& name, int value);
    // Set a uniform float value
    bool SetUniform(const std::string& name, float value);
    // Set a uniform Vec2 value
    bool SetUniform(const std::string& name, const glm::vec2& value);
    // Set a uniform Vec3 value
    bool SetUniform(const std::string& name, const glm::vec3& value);
    // Set a uniform Vec4 value
    bool SetUniform(const std::string& name, const glm::vec4& value);
    // Set a uniform Mat4 value
    bool SetUniform(const std::string& name, const glm::mat4& value);

protected:
    bool LoadSource(const std::string& filepath, std::string& shaderSource);
    
    GLuint Compile(GLenum shaderType, const char* shaderSource);

    GLuint Link(std::vector<GLuint> programs);

private:
    void ParseVertexAndFragment(const std::string& input, std::string& vertexShader, std::string& fragmentShader);
};