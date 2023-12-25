#pragma once

#include "Agent.h"
#include <glm/glm.hpp>
#include <vector>

#include "ComputeBuffer1D.h"
#include "ComputeObject.h"
#include "RenderTexture.h"

class Simulation {
protected:
  glm::vec2 simulationDimension;

  ptr<ComputeBuffer1D<Agent>> agents;
  ComputeObject simulation;

public:
  Simulation();

  void Init(const std::vector<Agent> agents);

  // Add an agent to the simulation
  // void AddAgent(const glm::vec2& initialPosition, const glm::vec2&
  // initialVelocity);

  // Update the simulation state
  void Update(float deltaTime);

  void Step() { simulation.Step(); }
  void Stop() { simulation.Stop(); }
  void Start() { simulation.Start(); }

  // Render the simulation to canvas
  void Render(const Renderer &ren);

  void SetSize(glm::vec2 size) { simulationDimension = size; }

  void ImGuiRender();
};