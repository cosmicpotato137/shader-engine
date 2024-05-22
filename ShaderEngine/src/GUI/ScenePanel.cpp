#include "pch.h"
#include "ScenePanel.h"

std::vector<ScenePanel *> ScenePanel::m_ScenePanels;
ScenePanel *ScenePanel::m_FocusedScenePanel = nullptr;
ScenePanel *ScenePanel::m_LastHoveredScenePanel = nullptr;

ScenePanel::ScenePanel() : Panel("Scene") {
  // Add to list of scene panels
  m_ScenePanels.push_back(this);
}

ScenePanel::~ScenePanel() {
  // Remove from list of scene panels
  m_ScenePanels.erase(
      std::remove(m_ScenePanels.begin(), m_ScenePanels.end(), this),
      m_ScenePanels.end());
}

void ScenePanel::SetRenderer(ptr<Renderer> renderer) { m_Renderer = renderer; }

void ScenePanel::ImGuiRender() {
  if (!m_Renderer) {
    return;
  }

  // Scene window styling
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {3.0, 3.0});

  SceneWindow();
  if (m_Debug) {
    DebugSwapTargetWindow();
  }

  ImGui::PopStyleVar(3);
}

void ScenePanel::SceneWindow() {
  ImGui::Begin("Scene");

  // Update render target size if needed
  m_PanelContentSize = ImGui::GetContentRegionAvail();
  if (ImGui::GetWindowSize() != m_Size) {
    m_Renderer->Init(m_PanelContentSize.x, m_PanelContentSize.y);
  }

  // Get Size
  m_Size = ImGui::GetWindowSize();

  // Get window position
  m_Position = ImGui::GetWindowPos();

  // Get mouse positon
  ImVec2 mousePos = ImGui::GetMousePos();
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  m_PanelCursorPos = mousePos - cursorPos;

  // Get focused and hovered panels
  m_FocusedScenePanel = ImGui::IsWindowFocused() ? this : m_FocusedScenePanel;
  m_LastHoveredScenePanel =
      ImGui::IsWindowHovered() ? this : m_LastHoveredScenePanel;

  // Get the texture ID
  int textureID = m_Renderer->GetRenderTarget()->GetTexture()->GetTextureID();

  // Render image to layer
  ImGui::GetWindowDrawList()->AddImage(
      reinterpret_cast<void *>(static_cast<intptr_t>(textureID)),
      ImVec2(cursorPos),
      ImVec2(
          cursorPos.x + m_PanelContentSize.x,
          cursorPos.y + m_PanelContentSize.y),
      ImVec2(0, 1),
      ImVec2(1, 0));

  ImGui::End();  // Scene
}

void ScenePanel::DebugSwapTargetWindow() {
  // DEBUG SWAP TARGET WINDOW
  ImGui::Begin("Scene - debug swap target");

  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  int textureID = m_Renderer->GetSwapTarget()->GetTexture()->GetTextureID();
  // Render image to layer
  ImGui::GetWindowDrawList()->AddImage(
      reinterpret_cast<void *>(static_cast<intptr_t>(textureID)),
      ImVec2(cursorPos),
      ImVec2(cursorPos.x + windowSize.x, cursorPos.y + windowSize.y),
      ImVec2(0, 1),
      ImVec2(1, 0));
  ImGui::End();
}
