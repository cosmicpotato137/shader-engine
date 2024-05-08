#include "EditorLayer.h"
#include "Core/Log.h"
#include "Core/Application.h"

EditorLayer::EditorLayer() : ApplicationLayer("Editor Layer") {
  m_Renderer = std::make_shared<Renderer>();
  m_ComputeShader = std::make_shared<ComputeShader>("Compute Shader");
  m_ScenePanel.SetRenderer(m_Renderer);
  m_ShaderPanel.SetShader(m_ComputeShader);
}

EditorLayer::~EditorLayer() {}

bool EditorLayer::OnAttach() {
  Console::Log("Welcome to the Shader Sandbox!");
  m_Renderer->Init(100, 100);

  std::filesystem::path path = m_DataPath / "mandelbrot.dat";
  // if path exists, load it
  if (std::filesystem::exists(path)) {
    Serial::LoadInplace(m_ComputeShader, path.string());
  } else {
    m_ComputeShader->SetUniform("_center", glm::vec2{0, 0});
    m_ComputeShader->SetUniform("_scale", 2.0f);
    m_ComputeShader->SetUniform("max_iterations", GLuint(20));
    m_ComputeShader->SetUniform("color", glm::vec3{1, 1, 1});
  }
  m_ComputeShader->Init(
      std::string(SHADER_DIR) + "/compute/mandelbrot.compute");

  return true;
}

void EditorLayer::OnDetach() {
  std::filesystem::path path = m_DataPath / "mandelbrot.dat";
  Serial::Save(m_ComputeShader, path.string());
  m_ComputeShader->Cleanup();
  m_Renderer->Cleanup();
}

void EditorLayer::ImGuiRender() {
  m_ScenePanel.ImGuiRender();
  m_OutputPanel.ImGuiRender();
  m_ShaderPanel.ImGuiRender();
}

void EditorLayer::Render() {
  glm::vec2 size = m_Renderer->GetViewportSize();
  m_ComputeShader->SetWorkGroups(size.x / 8, size.y / 8, 1);

  // Note texture and uniform are bound to the same value
  m_Renderer->GetRenderTarget()->GetTexture()->BindCompute(0);
  m_ComputeShader->SetUniform("imageOut", 0);

  m_ComputeShader->Use();
}
