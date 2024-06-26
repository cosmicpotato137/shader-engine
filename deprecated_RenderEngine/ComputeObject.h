#pragma once

#include "ComputeShader.h"
#include "RenderTexture.h"
#include "Object.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

class ComputeObject : public Object {
  std::string name;
  ptr<ComputeShader> shader;
  char *shaderPath = new char[256];
  IGFD::FileDialogConfig config;
  IGFD::FileDialog fileDialog;

  bool pause = false, reset = false, step = false;

public:
  ComputeObject(const std::string &name, const std::string &shaderPath);

  void SetShader(const std::string &shaderPath);
  ptr<ComputeShader> GetShader() const { return shader; }
  void RecompileShader();

  void Step();
  void ToggleStop();
  void Start();
  void Stop();
  bool IsReset() const { return reset; }

  void Render();
  void ImGuiRender();


private:
  void ShaderInfoWindow();
};