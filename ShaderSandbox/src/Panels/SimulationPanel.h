#pragma once
#include "GUI/ComputeShaderPanel.h"
#include "Renderer/ComputeBuffer1D.h"
#include "Simulation/Agent.h"
#include "Simulation/SimulationSpecification.h"
#include "Core/Serial.h"

#include <functional>

class SimulationPanel : public ComputeShaderPanel {
  ptr<ComputeBuffer1D<Agent>> m_Agents;

  // Init specification for simulation
  SimulationSpecification m_Specification;

public:
  SimulationPanel(std::shared_ptr<ComputeShader> shader = nullptr);

protected:
  virtual void SetWorkGroups(const Renderer &renderer) override;
  virtual void SetRenderTargets(const Renderer &renderer) override;

  virtual void OnReset() override;
  void ResetSimulation();

  // Boost serialization
  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    // Serialize base class
    ar &boost::serialization::base_object<ComputeShaderPanel>(*this);
    ar & m_Specification;
  }
};
