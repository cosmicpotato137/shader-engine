
#include "glm/glm.hpp"
#include "Agent.h"
#include "Core/Serial.h"
#include <vector>

struct SimulationSpecification {
  int NumAgents = 100;

  virtual std::vector<Agent> InitAgents();

  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & NumAgents;
  }
};