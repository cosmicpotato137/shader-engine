#include "Agent.h"

Agent::Agent(const glm::vec2& initialPosition, const glm::vec2& initialVelocity)
    : position(initialPosition), velocity(initialVelocity) {}

// Update the agent's position based on its velocity
void Agent::Update(float dt) {
    lastPosition = position;
    position += velocity;
}

// Setters for position and velocity
void Agent::SetPosition(const glm::vec2& newPosition) {
    position = newPosition;
}

void Agent::SetVelocity(const glm::vec2& newVelocity) {
    velocity = newVelocity;
}

// Getters for position and velocity
const glm::vec2& Agent::GetPosition() const {
    return position;
}

const glm::vec2& Agent::GetVelocity() const {
    return velocity;
}

const glm::vec2 Agent::GetLastPosition() const
{
    return lastPosition;
}
