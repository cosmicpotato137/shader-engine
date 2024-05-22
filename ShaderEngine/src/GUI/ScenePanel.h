#pragma once

#include "GUI/Panel.h"
#include "Renderer/Renderer.h"
#include <vector>

class ScenePanel : public Panel {
  ptr<Renderer> m_Renderer;
  bool m_Debug = false;
  ImVec2 m_PanelCursorPos = {0, 0};
  ImVec2 m_PanelContentSize = {0, 0};

  static std::vector<ScenePanel *> m_ScenePanels;
  static ScenePanel *m_FocusedScenePanel;
  static ScenePanel *m_LastHoveredScenePanel;

public:
  ScenePanel();
  ~ScenePanel() override;

  void SetRenderer(ptr<Renderer> renderer);

  void ImGuiRender();

  ImVec2 GetPanelCursorPos() { return m_PanelCursorPos; }
  ImVec2 GetPanelContentSize() { return m_PanelContentSize; }

  static ScenePanel *GetScenePanel(int index) { return m_ScenePanels[index]; }
  static ScenePanel *GetFocusedScenePanel() { return m_FocusedScenePanel; }
  static ScenePanel *GetLastHoveredScenePanel() {
    return m_LastHoveredScenePanel;
  }

private:
  void SceneWindow();
  void DebugSwapTargetWindow();
};