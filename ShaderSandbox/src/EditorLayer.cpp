#include "EditorLayer.h"
#include "Core/Log.h"
#include "Core/Application.h"

EditorLayer::EditorLayer() : ApplicationLayer("Editor Layer") {
  m_Renderer = std::make_shared<Renderer>();
  m_ComputeShader = std::make_shared<ComputeShader>("Compute Shader");
  m_ScenePanel.SetRenderer(m_Renderer);
  m_ComputeShaderStackPanel.AddComputeShader(m_ComputeShader);
}
EditorLayer::~EditorLayer() {}

bool EditorLayer::OnAttach() {
  Console::Log("Welcome to the Shader Sandbox!");
  m_Renderer->Init(100, 100);

  std::string shaderPath =
      std::string(SHADER_DIR) + "/compute/mandelbrot.compute";
  m_ComputeShader->Init(shaderPath);
  // If path exists, load it, otherwise, init values
  std::filesystem::path path =
      m_DataPath / (m_ComputeShader->GetName() + ".dat");
  if (std::filesystem::exists(path)) {
    Serial::LoadInplace(m_ShaderPanel, path.string());
  } else {
    m_ComputeShader->SetUniform("_center", glm::vec2{0, 0});
    m_ComputeShader->SetUniform("_scale", 2.0f);
    m_ComputeShader->SetUniform("max_iterations", GLuint(20));
    m_ComputeShader->SetUniform("color", glm::vec3{1, 1, 1});
  }

  return true;
}

void EditorLayer::OnDetach() {
  std::filesystem::path path =
      m_DataPath / (m_ComputeShader->GetName() + ".dat");
  Serial::Save(m_ShaderPanel, path.string());
  m_ComputeShader->Cleanup();
  m_Renderer->Cleanup();
}

void EditorLayer::ImGuiRender() {
  m_ScenePanel.ImGuiRender();
  m_OutputPanel.ImGuiRender();
  m_ComputeShaderStackPanel.ImGuiRender();
}

void EditorLayer::Render() {
  m_ShaderPanel.SetRenderTargets(*m_Renderer);
  m_ShaderPanel.Render();
  m_Renderer->Swap();
}

void EditorLayer::Update(double dt) { m_ShaderPanel.Update(dt); }
