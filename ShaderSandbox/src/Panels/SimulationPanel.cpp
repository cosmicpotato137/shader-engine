#pragma once

#include "SimulationPanel.h"

SimulationPanel::SimulationPanel(
    std::shared_ptr<ComputeShader> shader,
    std::function<std::vector<Agent>()> setAgents)
    : ComputeShaderPanel(shader) {
  m_Agents = std::make_shared<ComputeBuffer1D<Agent>>();

  if (setAgents != nullptr) {
    m_SetAgents = setAgents;
  }

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

void SimulationPanel::ResetSimulation() {
  if (m_SetAgents != nullptr) {
    auto agents = m_SetAgents();
    m_Agents->InitFromVector(agents);
  }
}

void SimulationPanel::SetSetAgentsCallback(
    std::function<std::vector<Agent>()> callback) {
  m_SetAgents = callback;
}