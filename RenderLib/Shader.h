#pragma once 

#include "core.h"
#include <variant>

typedef std::variant<bool, GLint, GLuint, GLfloat, glm::vec2,
    glm::vec3, glm::vec4, glm::mat4> u_types;

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
    
    bool SetUniform(const std::string& uniform_name,
        const u_types& value);

protected:
    bool LoadSource(const std::string& filepath, std::string& shaderSource);
    
    GLuint Compile(GLenum shaderType, const char* shaderSource);

    GLuint Link(std::vector<GLuint> programs);

private:
    void ParseVertexAndFragment(const std::string& input, std::string& vertexShader, std::string& fragmentShader);
};