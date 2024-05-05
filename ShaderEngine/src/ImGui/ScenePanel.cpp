#include "ScenePanel.h"

void ScenePanel::ImGuiRender() {
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

void ScenePanel::OnResize() {
  m_Renderer->GetRenderTarget()->Init(m_Size.x, m_Size.y, false);
  m_Renderer->GetSwapTarget()->Init(m_Size.x, m_Size.y, false);
}

void ScenePanel::SceneWindow() {
  ImGui::Begin("Scene");
  ImVec2 imContentSize = ImGui::GetContentRegionAvail();

  if (imContentSize != m_Size) {
    m_Size = imContentSize;
    OnResize();
  }

  // Get the texture ID
  int textureID = m_Renderer->GetRenderTarget()->GetTexture()->GetTextureID();

  // Render image to layer
  ImVec2 windowPos = ImGui::GetCursorScreenPos();
  ImGui::GetWindowDrawList()->AddImage(
      reinterpret_cast<void *>(static_cast<intptr_t>(textureID)),
      ImVec2(windowPos),
      ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
      ImVec2(0, 1),
      ImVec2(1, 0));

  ImGui::End();  // Scene
}

void ScenePanel::DebugSwapTargetWindow() {
  // DEBUG SWAP TARGET WINDOW
  ImGui::Begin("Scene - debug swap target");

  ImVec2 windowPos = ImGui::GetCursorScreenPos();
  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  int textureID = m_Renderer->GetSwapTarget()->GetTexture()->GetTextureID();
  // Render image to layer
  ImGui::GetWindowDrawList()->AddImage(
      reinterpret_cast<void *>(static_cast<intptr_t>(textureID)),
      ImVec2(windowPos),
      ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
      ImVec2(0, 1),
      ImVec2(1, 0));
  ImGui::End();
}
