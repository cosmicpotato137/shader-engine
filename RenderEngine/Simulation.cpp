#include "Simulation.h"
#include <iostream>

Simulation::Simulation()
    : simulation(
          "boids",
          std::string(SHADER_DIR) + "/compute/simulation/slime.compute") {
  this->agents = std::make_shared<ComputeBuffer1D<Agent>>();
}

void Simulation::Init(const std::vector<Agent> agents) {
  this->agents->InitFromVector(agents);
  simulation.GetShader()->SetWorkGroups(
      std::ceil(agents.size() * sizeof(Agent) / 256), 1, 1);
}

// Void Simulation::AddAgent(const glm::vec2& initialPosition, const glm::vec2&
// InitialVelocity) {
//    agents->PushBack(Agent{ initialPosition, initialVelocity });
//    simulation.GetShader()->SetWorkGroups(std::ceil(agents->GetSize() *
//    sizeof(Agent) / 8), 1, 1);
//}

// Function to find k-nearest neighbors for a given agent in a list of agents
std::vector<Agent> FindKNearestNeighbors(
    const Agent &targetAgent, const std::vector<Agent> &agentList, int k) {
  // Create a vector to store distances and corresponding agents
  std::vector<std::pair<float, Agent>> distancesAndAgents;

  // Calculate distances and store in the vector
  for (const auto &agent : agentList) {
    float distance =
        glm::distance(targetAgent.GetPosition(), agent.GetPosition());
    distancesAndAgents.emplace_back(distance, agent);
  }

  // Sort the vector based on distances
  std::sort(
      distancesAndAgents.begin(),
      distancesAndAgents.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.first < rhs.first; });

  // Extract the k-nearest neighbors
  std::vector<Agent> kNearestNeighbors;
  for (int i = 1; i < k + 1 && i < distancesAndAgents.size(); ++i) {
    kNearestNeighbors.push_back(distancesAndAgents[i].second);
  }

  return kNearestNeighbors;
}

void Simulation::Update(float deltaTime) {
  // If (simulationDimension.y + simulationDimension.x == 0)
  //    return;

  // For (Agent& agent : agents) {
  //    glm::vec2 pos = agent.GetPosition();
  //    glm::vec2 lastPos = agent.GetLastPosition();
  //    glm::vec2 vel = agent.GetVelocity();

  //    // Turn towards neighbors
  //    auto neighbors = FindKNearestNeighbors(agent, agents, 4);
  //    glm::vec2 sumDir = glm::vec2(0);
  //    glm::vec2 sumVel = glm::vec2(0);
  //    for (Agent& n : neighbors)
  //    {
  //        glm::vec2 dir = glm::normalize(n.GetPosition() - pos);
  //        sumVel += n.GetVelocity();
  //        if (glm::distance(n.GetPosition(), pos) < 20)
  //            sumDir -= dir;
  //        else
  //            sumDir += dir;
  //    }
  //    vel += glm::normalize(sumDir);
  //    vel += glm::normalize(sumVel);

  //    // Clamp velocity;
  //    float maxLen = 5;
  //    if (glm::length(vel) > maxLen);
  //        vel = glm::normalize(vel) * maxLen;

  //    // Flip velocity if agent is at the edge of the texture
  //    if (pos.x < 0)
  //    {
  //        float slope = (pos.y - lastPos.y) / (pos.x - lastPos.x);
  //        pos.y += abs(pos.x) * slope;
  //        pos.x = 0;
  //        vel.x = abs(vel.x);
  //    }
  //    else if (pos.x > simulationDimension.x)
  //    {
  //        float slope = (pos.y - lastPos.y) / (pos.x - lastPos.x);
  //        pos.y -= abs(simulationDimension.x - pos.x) * slope;
  //        pos.x = simulationDimension.x;
  //        vel.x = -abs(vel.x);
  //    }

  //    if (pos.y < 0 )
  //    {
  //        float slope = (pos.x - lastPos.x) / (pos.y - lastPos.y);
  //        pos.x += abs(pos.y) * slope;
  //        pos.y = 0;
  //        vel.y = abs(vel.y);
  //    }
  //    else if (pos.y > simulationDimension.y)
  //    {
  //        float slope = (pos.x - lastPos.x) / (pos.y - lastPos.y);
  //        pos.x -= abs(simulationDimension.y - pos.y) * slope;
  //        pos.y = simulationDimension.y;
  //        vel.y = -abs(vel.y);
  //    }

  //    agent.SetVelocity(vel);
  //    agent.SetPosition(pos);
  //    agent.Update(deltaTime);
  //}
}

void Simulation::Render(const Renderer &ren) {
  // Bind agent buffer
  agents->BindCompute(0);

  // Bind swap target
  auto shader = simulation.GetShader();
  shader->SetUniform("imageOut", 0);
  ren.GetSwapTarget()->GetTexture()->BindCompute(0);

  // Bind render target
  shader->SetUniform("imageIn", 1);
  ren.GetRenderTarget()->GetTexture()->BindCompute(1);

  // Render simulation
  simulation.Render();
}

void Simulation::ImGuiRender() { simulation.ImGuiRender(); }
