#pragma once

#include "Core/core.h"
#include "Core/Serial.h"
#include "ShaderUniform.h"

class Shader {
protected:
  GLuint program;
  std::string filepath;
  std::string name;

  std::map<std::string, GLuint> uniformLocations;
  std::map<std::string, ptr<Uniform>> uniforms;

public:
  Shader() : program(0), name(""), filepath("") {}
  Shader(const std::string &name) : program(0), name(name), filepath("") {}
  ~Shader() { Cleanup(); }

  bool Init(const std::string &shaderPath);
  virtual bool InitFromSource(const std::string &shaderSource);

  // Use the shader program
  virtual void Use();
  void Drop() const { glUseProgram(0); }

  void Cleanup();

  // Find the integer location of a uniform
  GLint GetUniformLocation(const std::string &name);

  GLuint GetProgramID() const;
  std::string GetFilePath() const;

  // Get the name of the shader
  std::string GetName() const;

  // Set a uniform value
  void SetUniform(const std::string &uniformName, const uniform_types &value);

  // Get a uniform
  ptr<Uniform> GetUniform(const std::string &uniformName);

  // Check if a uniform exists
  bool HasUniform(const std::string &uniformName);

  // Iterator for uniforms
  std::map<std::string, ptr<Uniform>>::iterator begin();
  std::map<std::string, ptr<Uniform>>::iterator end();

  // operator for shader uniforms
  ptr<Uniform> operator[](const std::string &uniformName);
  ptr<Uniform> operator[](const char *uniformName);

protected:
  // Shader cimpilation and linking
  bool LoadSource(const std::string &filepath, std::string &shaderSource);
  GLuint Compile(GLenum shaderType, const char *shaderSource);
  GLuint Link(std::vector<GLuint> programs);

  void FindUniforms();
  void ApplyUniforms();

private:
  void ParseVertexAndFragment(
      const std::string &input, std::string &vertexShader,
      std::string &fragmentShader);

  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & name;
    ar & filepath;
    ar & uniforms;
    if (Archive::is_loading::value && std::filesystem::exists(filepath)) {
      Init(filepath);
    }
  }
};