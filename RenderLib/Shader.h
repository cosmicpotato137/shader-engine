#pragma once

#include "core.h"
#include "ShaderUniform.h"

class Shader {
protected:
  GLuint program;
  std::string filepath;

  std::map<std::string, GLuint> uniformLocations;

public:
  std::string name;
  std::map<std::string, ptr<Uniform>> uniforms;

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

  // Set a uniform value
  void SetUniform(const std::string &uniformName, const uniform_types &value);
  // Get a uniform
  ptr<Uniform> GetUniform(const std::string &uniformName);
  // Check if a uniform exists
  bool HasUniform(const std::string &uniformName) const;

  // Serialization
  friend std::ostream &operator<<(std::ostream &os, const Shader &shader);

  // Deserialization
  friend std::istream &operator>>(std::istream &is, Shader &shader);

protected:
  bool LoadSource(const std::string &filepath, std::string &shaderSource);

  GLuint Compile(GLenum shaderType, const char *shaderSource);

  GLuint Link(std::vector<GLuint> programs);

  void FindUniforms();
  void ApplyUniforms();

private:
  void ParseVertexAndFragment(
      const std::string &input, std::string &vertexShader,
      std::string &fragmentShader);
};