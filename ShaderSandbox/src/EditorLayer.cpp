#include "EditorLayer.h"
#include "Core/Log.h"

EditorLayer::EditorLayer() : ApplicationLayer("Editor Layer") {
  m_Renderer = std::make_shared<Renderer>();
  m_ScenePanel.SetRenderer(m_Renderer);
}

EditorLayer::~EditorLayer() {}

bool EditorLayer::OnAttach() {
  Console::Log("Welcome to the Shader Sandbox!");
  m_Renderer->Init(100, 100);
  return true;
}

void EditorLayer::OnDetach() { m_Renderer->Cleanup(); }

void EditorLayer::ImGuiRender() {
  m_ScenePanel.ImGuiRender();
  m_OutputPanel.ImGuiRender();
}
