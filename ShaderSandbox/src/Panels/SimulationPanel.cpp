#include "SimulationPanel.h"

BOOST_CLASS_EXPORT_GUID(SimulationPanel, "SimulationPanel")

SimulationPanel::SimulationPanel(std::shared_ptr<ComputeShader> shader)
    : ComputeShaderPanel(shader) {
  m_Agents = std::make_shared<ComputeBuffer1D<Agent>>();

  ResetSimulation();
}

void SimulationPanel::SetWorkGroups(const Renderer &renderer) {
  if (m_Shader == nullptr)
    return;

  int size = m_Agents->GetSize();
  m_Shader->SetWorkGroups(std::ceil(size * sizeof(Agent) / 256), 1, 1);
}

void SimulationPanel::SetRenderTargets(const Renderer &renderer) {
  if (m_Shader == nullptr)
    return;

  // Bind render targets
  ComputeShaderPanel::SetRenderTargets(renderer);

  // Bind agent buffer
  m_Agents->BindCompute(0);
}

void SimulationPanel::OnReset() {
  ImGui::DragInt("Number of Agents", &m_Specification.NumAgents, 1, 10000);
  if (ImGui::Button("Reset Simulation", ImVec2(150, 25))) {
    m_Time = 0.0f;
    m_Reset = true;
    ResetSimulation();
  }
}

void SimulationPanel::ResetSimulation() {
  auto agents = m_Specification.InitAgents();
  m_Agents->InitFromVector(agents);
}
