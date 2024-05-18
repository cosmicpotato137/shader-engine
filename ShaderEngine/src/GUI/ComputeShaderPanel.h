#pragma once

#include "GUI/Panel.h"
#include "Renderer/ComputeShader.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderTexture.h"
#include "Core/Serial.h"
#include "Core/FileDialog.h"

class ComputeShaderPanel : public Panel {
public:
  ComputeShaderPanel(std::shared_ptr<ComputeShader> shader = nullptr);
  ComputeShaderPanel(const ComputeShaderPanel &other);

  void ImGuiRender() override;
  void Render(const Renderer &ren) override;
  void Update(double dt) override;

  void SetShader(std::shared_ptr<ComputeShader> shader);
  // void SetRenderTargets(
  // const RenderTexture &renderTarget, const RenderTexture &swapTarget);

  ComputeShaderPanel &operator=(const ComputeShaderPanel &other);

protected:
  // Can be overriden to give different render targets
  virtual void SetRenderTargets(const Renderer &renderer);
  virtual void SetWorkGroups(const Renderer &renderer);
  void UniformVisitor();
  void HeaderOperations();
  void CustomUniforms();

  // Boost serailization
  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    // Serialize base class
    ar &boost::serialization::base_object<Panel>(*this);
    ar & m_Shader;
  }

  FileDialog m_FileDialog = FileDialog(
      {std::pair<std::string, std::string>{"Compute Shader", "compute"}});

  std::shared_ptr<ComputeShader> m_Shader;
  bool m_IsOpen = true;
  bool m_Step = false;
  bool m_Pause = false;
  bool m_Reset = false;
  float m_Time = 0.0f;
};