#include "Shader.h"

#include <fstream>
#include <sstream>
#include <cstdio>
#include <regex>

#include "Log.h"


bool Shader::Init(const std::string & shaderPath)
{
    // Load shader source code from files
    std::string shaderSource;
    if (!LoadSource(shaderPath, shaderSource))
    {
        return false;
    }

    return InitFromSource(shaderSource);
}

bool Shader::InitFromSource(const std::string & shaderSource)
{
    std::string vertexSource, fragmentSource;
    ParseVertexAndFragment(shaderSource, vertexSource, fragmentSource);

    // Compile and link shaders
    GLuint vertexShader = Compile(GL_VERTEX_SHADER, vertexSource.c_str());
    GLuint fragmentShader = Compile(GL_FRAGMENT_SHADER, fragmentSource.c_str());

    if (!vertexShader || !fragmentShader)
    {
        return false;
    }

    program = Link({ vertexShader, fragmentShader });

    if (!program)
    {
        return false;
    }

    FindUniforms();

    return true;
}

void Shader::Use()
{
    glUseProgram(program);
    ApplyUniforms();
}

void Shader::Cleanup()
{
    if (program != 0)
    {
        glDeleteProgram(program);
        program = 0;
    }
}

// deprecated
GLint Shader::GetUniformLocation(const std::string& name)
{
    // try to find the uniform
    if (uniformLocations.find(name) != uniformLocations.end())
        return uniformLocations[name];

    // set the uniform if not found
    GLuint loc = glGetUniformLocation(program, name.c_str());
    if (loc == -1)
        Console::Warning("%s: Uniform '%s' not found", this->name.c_str(), name.c_str());

    uniformLocations.insert({ name, loc });
    return loc;
}

void Shader::SetUniform(const std::string& name, const uniform_types& value)
{
    if (uniforms.find(name) != uniforms.end())
        uniforms[name]->SetValue(value);
    else
    {
        Console::Warning("%s: uniform '%s' not found", this->name.c_str(), name.c_str());
        uniforms[name] = std::make_shared<Uniform>(name, -1, 0);
    }
}

std::unordered_map<std::string, ptr<Uniform>> Shader::GetUniforms()
{
    return uniforms;
}

void Shader::FindUniforms()
{
    int count = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++)
    {
        int length, size;
        GLuint type;
        char* name = new char[255];
        glGetActiveUniform(program, (GLuint)i, (GLsizei)255, &length, &size, &type, name);

        uniforms[std::string(name)] = glToShaderUniform(name, glGetUniformLocation(program, name), type, size);
    }
}

void Shader::ApplyUniforms() {
    for (auto iter = uniforms.begin(); iter != uniforms.end(); ++iter)
    {
        const uniform_types value = iter->second->GetValue();

        // set value
        struct Visitor {
            ptr<Uniform> uniform;
            int program;
            int location;
            std::string programName;

            Visitor(ptr<Uniform> uniform, int program, const std::string& programName) 
                : uniform(uniform), program(program), programName(programName) 
            {
                location = uniform->GetLocation();
            }

            void operator()(bool value) {
                if (uniform->GetType() != Bool)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform1i(program, location, value);
            }
            void operator()(GLint value) {
                bool typecheck = false;
                typecheck |= uniform->GetType() == Int;
                typecheck |= uniform->GetType() == Image2D;
                typecheck |= uniform->GetType() == Texture2D;
                if (!typecheck)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform1i(program, location, value);
            }
            void operator()(GLuint value) {
                bool typecheck = false;
                typecheck |= uniform->GetType() == Int;
                typecheck |= uniform->GetType() == Image2D;
                typecheck |= uniform->GetType() == Texture2D;
                if (!typecheck)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform1ui(program, location, value);
            }
            void operator()(GLfloat value) {
                if (uniform->GetType() != Float)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform1f(program, location, value);
            }
            void operator()(const glm::vec2& value) {
                if (uniform->GetType() != Vec2)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
            }
            void operator()(const glm::vec3& value) {
                bool typecheck = false;
                typecheck |= uniform->GetType() != Vec3;
                typecheck |= uniform->GetType() != Col3;
                if (!typecheck)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
            }
            void operator()(const glm::vec4& value) {
                bool typecheck = false;
                typecheck |= uniform->GetType() != Vec4;
                typecheck |= uniform->GetType() != Col4;
                if (!typecheck)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniform4fv(program, location, 1, glm::value_ptr(value));
            }
            void operator()(const glm::mat4& value) {
                if (uniform->GetType() != Mat4)
                    Console::Error("%s: uniform '%s' assigned to the wrong type", programName.c_str(), uniform->GetName().c_str());
                glProgramUniformMatrix4fv(program, location, 1, GL_FALSE,
                    glm::value_ptr(value));
            }
        };
        std::visit(Visitor(iter->second, program, this->name), value);
    }

}

bool Shader::LoadSource(const std::string& filepath, std::string& shaderSource)
{
    std::string fileContents;
    std::ifstream fileStream(filepath, std::ios::in);

    if (fileStream.is_open()) {
        std::string line;
        while (std::getline(fileStream, line)) {
            fileContents += line + "\n"; // Append each line to the contents
        }
        fileStream.close();
    }
    else {
        Console::Error("Unable to open file: %s", filepath.c_str());
        return false;
    }

    shaderSource = fileContents;

    return true;
}

GLuint Shader::Compile(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);

    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        Console::Error("%s: Shader compilation error: \n%s\n", name.c_str(), &errorLog[0]);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint Shader::Link(std::vector<GLuint> programs)
{
    GLuint program = glCreateProgram();
    for (GLuint p : programs)
        glAttachShader(program, p);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
        Console::Error("%s : Shader program linking error", name.c_str());
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void Shader::ParseVertexAndFragment(const std::string& input, std::string& vertexShader, std::string& fragmentShader)
{
    std::string vdelimiter = "#vertex\n";  // The tag indicating the start of the vertex shader
    size_t vertexPos = input.find(vdelimiter);
    if (vertexPos == std::string::npos) {
        Console::Error("%s: Vertex shader tag not found:", name.c_str());
        return;
    }
    std::string fdelimiter = "#fragment\n";  // The tag indicating the start of the fragment shader
    size_t fragmentPos = input.find(fdelimiter);
    if (fragmentPos == std::string::npos)
    {
        Console::Error("%s: Fragment shader tag not found:", name.c_str());
        return;
    }
    if (fragmentPos < vertexPos)
    {
        Console::Error("%s: Vertex shader must appear before fragment shader", name.c_str());
        return;
    }

    vertexShader = input.substr(vertexPos + vdelimiter.length(), fragmentPos - vdelimiter.length() - 1);
    fragmentShader = input.substr(fragmentPos + fdelimiter.length());
}

ptr<Uniform> glToShaderUniform(const char* name, int location, GLuint type, GLsizei size)
{
    // check inspector hide/show
    std::regex re("_?((color)|(c_))[a-zA-Z]*");

    bool hidden = false;
    std::regex re1("_.*");
    if (std::regex_search(name, re1))
        hidden = true;

    switch (type)
    {
    case GL_BOOL:           return std::make_shared<Uniform>(std::string(name), location, false, Bool, hidden);
    case GL_INT:            return std::make_shared<Uniform>(std::string(name), location, 0, Int, hidden);
    case GL_UNSIGNED_INT:   return std::make_shared<Uniform>(std::string(name), location, 0, UInt, hidden);
    case GL_FLOAT:          return std::make_shared<Uniform>(std::string(name), location, 0.0f, Float, hidden);
    case GL_FLOAT_VEC2:     return std::make_shared<Uniform>(std::string(name), location, glm::vec2(0), Vec2, hidden);
    case GL_FLOAT_VEC3:     
    {
        UniformType t = Vec3;
        if (std::regex_search(name, re))
            t = Col3;
        return std::make_shared<Uniform>(std::string(name), location, glm::vec3(0), t, hidden);
    }
    case GL_FLOAT_VEC4:     
    {
        UniformType t = Vec4;
        if (std::regex_search(name, re))
            t = Col4;
        return std::make_shared<Uniform>(std::string(name), location, glm::vec4(0), t, hidden);
    }
    case GL_FLOAT_MAT4:     return std::make_shared<Uniform>(std::string(name), location, glm::mat4(1), Mat4, true);
    case GL_SAMPLER_2D:     return std::make_shared<Uniform>(std::string(name), location, 0, Texture2D, true);
    case GL_IMAGE_2D:       return std::make_shared<Uniform>(std::string(name), location, 0, Image2D, true);
    default:
        Console::Error("GL type %u not found when creating uniform %s", type, name);
        assert(false);
    }
}
