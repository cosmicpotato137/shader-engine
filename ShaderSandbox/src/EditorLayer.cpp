#include "EditorLayer.h"
#include "Core/Log.h"
#include "Core/Application.h"

EditorLayer::EditorLayer() : ApplicationLayer("Editor Layer") {
  m_Renderer = std::make_shared<Renderer>();
  m_ScenePanel.SetRenderer(m_Renderer);
}
EditorLayer::~EditorLayer() {}

bool EditorLayer::OnAttach() {
  Console::Log("Welcome to the Shader Sandbox!");
  m_Renderer->Init(100, 100);

  // If path exists, load it, otherwise, init values
  std::filesystem::path path =
      m_DataPath / (m_ComputeShaderStackPanel.GetName() + ".dat");
  if (std::filesystem::exists(path)) {
    m_ComputeShaderStackPanel = Serial::Load<ComputeShaderStackPanel>(
        path.string());
  }
  return true;
}

void EditorLayer::OnDetach() {
  std::filesystem::path path =
      m_DataPath / (m_ComputeShaderStackPanel.GetName() + ".dat");
  Serial::Save(m_ComputeShaderStackPanel, path.string());
  m_Renderer->Cleanup();
}

void EditorLayer::ImGuiRender() {
  m_ScenePanel.ImGuiRender();
  m_OutputPanel.ImGuiRender();
  m_ComputeShaderStackPanel.ImGuiRender();
}

void EditorLayer::Render() {
  m_ComputeShaderStackPanel.Render(*m_Renderer);
  m_Renderer->Swap();
}

void EditorLayer::Update(double dt) { m_ComputeShaderStackPanel.Update(dt); }
