#pragma once

#include "ImGui/Panel.h"
#include "Renderer/Renderer.h"

class ScenePanel : public Panel {
  ptr<Renderer> m_Renderer;
  bool m_Debug = false;

public:
  ScenePanel() : Panel("Scene") {}
  ~ScenePanel() override = default;

  void SetRenderer(ptr<Renderer> renderer);

  void ImGuiRender();

  virtual void OnResize() override;

private:
  void SceneWindow();
  void DebugSwapTargetWindow();
};