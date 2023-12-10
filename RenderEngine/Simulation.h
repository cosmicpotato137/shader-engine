#pragma once

#include "Agent.h"
#include <vector>
#include <glm/glm.hpp>

#include "RenderTexture.h"

class Simulation {
protected:
    std::vector<Agent> agents;
    ptr<RenderTexture> canvas;
public:
    Simulation();

    void Init(ptr<RenderTexture> canvas);

    // Add an agent to the simulation
    void AddAgent(const glm::vec2& initialPosition, const glm::vec2& initialVelocity);

    // Update the simulation state
    void Update(float deltaTime);

    // Render the simulation to canvas
    void Render();
};