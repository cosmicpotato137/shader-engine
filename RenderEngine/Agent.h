#pragma once
#include "re_core.h"

class Agent {
public:
  Agent(const glm::vec2 &initialPosition, const glm::vec2 &initialVelocity);

  // Update the agent's position based on its velocity
  void Update(float dt);

  void SetPosition(const glm::vec2 &newPosition);

  void SetVelocity(const glm::vec2 &newVelocity);

  const glm::vec2 &GetPosition() const;

  const glm::vec2 &GetVelocity() const;

  const glm::vec2 GetLastPosition() const;

private:
  glm::vec2 position;
  glm::vec2 lastPosition;
  glm::vec2 velocity;
};
