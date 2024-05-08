#pragma once

#include "ImGui/Panel.h"
#include "Renderer/Shader.h"

class ShaderPanel : public Panel {
public:
  ShaderPanel(std::shared_ptr<Shader> shader = nullptr);
  void ImGuiRender() override;

  void SetShader(std::shared_ptr<Shader> shader);

private:
  std::shared_ptr<Shader> m_Shader;
  bool m_IsOpen = true;
  std::string m_ShaderPath = "";
  bool m_Step = false;
  bool m_Pause = false;
  bool m_Reset = false;
};