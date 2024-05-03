#include "pch.h"
#include "ComputeShader.h"

void ComputeShader::SetWorkGroups(
    int numGroupsX, int numGroupsY, int numGroupsZ) {
  x = numGroupsX;
  y = numGroupsY;
  z = numGroupsZ;
}

bool ComputeShader::InitFromSource(const std::string &computeSource) {
  // Compile and link shaders
  GLuint computeShader = Compile(GL_COMPUTE_SHADER, computeSource.c_str());

  if (!computeShader) {
    return false;
  }

  program = Link({computeShader});

  if (!program) {
    return false;
  }

  FindUniforms();

  return true;
}

void ComputeShader::Use() {
  Shader::Use();
  glDispatchCompute(x, y, z);
  // Make sure writing to image has finished before read
  glMemoryBarrier(memBarrier);
}
