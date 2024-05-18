#pragma once
#include "GUI/ComputeShaderPanel.h"
#include "Renderer/ComputeBuffer1D.h"
#include "Simulation/Agent.h"
#include <functional>

class SimulationPanel : public ComputeShaderPanel {
  ptr<ComputeBuffer1D<Agent>> m_Agents;

  // Set no agents by default
  std::function<std::vector<Agent>()> m_SetAgents = []() {
    return std::vector<Agent>();
  };

public:
  SimulationPanel(
      std::shared_ptr<ComputeShader> shader = nullptr,
      std::function<std::vector<Agent>()> setAgents = nullptr);

  virtual void SetWorkGroups(const Renderer &renderer) override;
  virtual void SetRenderTargets(const Renderer &renderer) override;

  void ResetSimulation();
  void SetSetAgentsCallback(std::function<std::vector<Agent>()> setAgents);
};