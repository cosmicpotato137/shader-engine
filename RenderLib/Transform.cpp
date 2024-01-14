#include "Transform.h"
#include "core.h"
#include "glm_helpers.h"

glm::mat4 Transform::GetWorldMatrix() const {
  // Calculate the world position matrix
  glm::mat4 worldMatrix = glm::mat4(1.0f);
  worldMatrix = glm::translate(worldMatrix, position);
  worldMatrix *= glm::mat4_cast(
      glm::quat(glm::radians(rotation)));  // Convert Euler angles to quaternion
  worldMatrix = glm::scale(worldMatrix, scale);
  return worldMatrix;
}

// Setters for position, rotation, and scale
void Transform::SetPosition(const glm::vec3 &newPosition) {
  position = newPosition;
}

void Transform::SetRotation(const glm::vec3 &newRotation) {
  rotation = newRotation;
}

void Transform::SetScale(const glm::vec3 &newScale) { scale = newScale; }

// Getters for position, rotation, and scale
glm::vec3 Transform::GetPosition() const { return position; }

glm::vec3 Transform::GetRotation() const { return rotation; }

glm::vec3 Transform::GetScale() const { return scale; }

std::ostream &operator<<(std::ostream &os, const Transform &t) {
  os << t.position;
  os << t.rotation;
  os << t.scale;

  return os;
}

std::istream &operator>>(std::istream &is, Transform &t) {
  is >> t.position;
  is >> t.rotation;
  is >> t.scale;

  return is;
}
