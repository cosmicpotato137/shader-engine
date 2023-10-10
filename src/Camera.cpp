#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : forward(glm::vec3(0.0f, 0.0f, -1.0f)), zoom(45.0f), fov(45.0f), nearPlane(0.1f), farPlane(100.0f)
{
    this->position = position;
    this->up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, forward, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect, bool perspective)
{
    return glm::perspective(glm::radians(45.0f), aspect, nearPlane, farPlane);
}

void Camera::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Camera::SetOrientation(float newYaw, float newPitch) {
    yaw = newYaw;
    pitch = newPitch;
    UpdateCameraVectors();
}

void Camera::LookAt(const glm::vec3& target) {
    glm::vec3 direction = glm::normalize(target - position);

    // Calculate pitch and yaw from the direction vector
    pitch = glm::degrees(asinf(direction.y));
    yaw = glm::degrees(atan2f(direction.z, direction.x));

    // Ensure pitch stays within the [-89, 89] degree range to prevent flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::SetZoom(float newZoom) {
    zoom = newZoom;
}

void Camera::SetFov(float fov)
{
    this->fov = fov;
}

void Camera::SetNearPlane(float nearPlane)
{
    this->nearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
    this->farPlane = farPlane;
}

float Camera::GetZoom() const {
    return zoom;
}

void Camera::UpdateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(newFront);
}
