#include "ComputeObject.h"
#include "Log.h"
#include "imgui.h"
#include "re_core.h"

ComputeObject::ComputeObject(
    const std::string &name, const std::string &shaderPath)
    : name(name) {
  shader = std::make_shared<ComputeShader>(name);
  this->shaderPath = strcpy(
      this->shaderPath,
      (std::string(SHADER_DIR) + "/compute/hello.compute").c_str());
  SetShader(shaderPath);
}

void ComputeObject::SetShader(const std::string &shaderPath) {
  std::string oldPath = this->shaderPath;
  shader->Drop();
  shader->Cleanup();
  // todo: this string handling is wack
  if (!shader->Init(shaderPath))
    shader->Init(oldPath);
  else {
    this->shaderPath = strcpy(this->shaderPath, shaderPath.c_str());
    Console::Log("Shader compiled sucessfully");
  }
}

void ComputeObject::RecompileShader() {
  std::string shaderPath = shader->GetFilePath();
  if (shader->Init(std::string(shaderPath)))
    Console::Log("Shader compiled sucessfully");
}

void ComputeObject::Step() { step = true; }

void ComputeObject::ToggleStop() { pause = !pause; }

void ComputeObject::Start() { pause = false; }

void ComputeObject::Stop() { pause = true; }

void ComputeObject::Render() {
  if (shader->HasUniform("_reset"))
    shader->SetUniform("_reset", reset);
  if (shader->HasUniform("_pause"))
    shader->SetUniform("_pause", pause && !step);

  // Deploy shader
  shader->Use();

  step = false;
  reset = false;
}

void ComputeObject::ImGuiRender() { ShaderInfoWindow(); }

void ComputeObject::ShaderInfoWindow() {}
