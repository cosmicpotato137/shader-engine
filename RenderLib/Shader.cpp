#include "Shader.h"

#include <fstream>
#include <sstream>
#include <cstdio>

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

    return true;
}

void Shader::Cleanup()
{
    if (program != 0)
    {
        glDeleteProgram(program);
        program = 0;
    }
}

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

bool Shader::SetUniform(const std::string& name, const u_types& value)
{
    // get location of uniform variable
    const GLint location = GetUniformLocation(name);
    if (location == -1)
        return false;

    // set value
    struct Visitor {
        GLuint program;
        GLint location;
        Visitor(GLuint program, GLint location)
            : program(program), location(location) {}

        void operator()(bool value) {
            glProgramUniform1i(program, location, value);
        }
        void operator()(GLint value) {
            glProgramUniform1i(program, location, value);
        }
        void operator()(GLuint value) {
            glProgramUniform1ui(program, location, value);
        }
        void operator()(GLfloat value) {
            glProgramUniform1f(program, location, value);
        }
        void operator()(const glm::vec2& value) {
            glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
        }
        void operator()(const glm::vec3& value) {
            glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
        }
        void operator()(const glm::vec4& value) {
            glProgramUniform4fv(program, location, 1, glm::value_ptr(value));
        }
        void operator()(const glm::mat4& value) {
            glProgramUniformMatrix4fv(program, location, 1, GL_FALSE,
                glm::value_ptr(value));
        }
    };
    std::visit(Visitor{ program, location }, value);
    return true;
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