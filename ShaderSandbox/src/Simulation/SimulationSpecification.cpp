#include "SimulationSpecification.h"

std::vector<Agent> SimulationSpecification::InitAgents() {
  std::vector<Agent> agents;
  int boxLength = ceil(sqrt(NumAgents));
  for (int i = 0; i < NumAgents; i++) {
    int x = i % boxLength;
    int y = i / boxLength;

    Agent agent(glm::vec2(x * 3, y * 3), glm::vec2(0, 0));
    agents.push_back(agent);
  }
  return agents;
}