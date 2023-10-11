#include "Transform.h"

glm::mat4 Transform::GetWorldMatrix() const {
    // Calculate the world position matrix
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    worldMatrix = glm::translate(worldMatrix, position);
    worldMatrix *= glm::mat4_cast(glm::quat(glm::radians(rotation))); // Convert Euler angles to quaternion
    worldMatrix = glm::scale(worldMatrix, scale);
    return worldMatrix;
}

// Setters for position, rotation, and scale
void Transform::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Transform::SetRotation(const glm::vec3& newRotation) {
    rotation = newRotation;
}

void Transform::SetScale(const glm::vec3& newScale) {
    scale = newScale;
}

// Getters for position, rotation, and scale
glm::vec3 Transform::GetPosition() const {
    return position;
}

glm::vec3 Transform::GetRotation() const {
    return rotation;
}

glm::vec3 Transform::GetScale() const {
    return scale;
}