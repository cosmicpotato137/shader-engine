#pragma once

#include "ComputeShader.h"
#include "RenderTexture.h"
#include "Object.h"

class ComputeObject : public Object {
  std::string name;
  ptr<ComputeShader> shader;
  char *shaderPath = new char[256];
  ptr<RenderTexture> imageIn;
  ptr<RenderTexture> imageOut;

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

protected:
  void Serialize(std::ofstream &ofs) override;
  void Deserialize(std::ifstream &ifs) override;

private:
  void ShaderInfoWindow();
};