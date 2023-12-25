#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
  glm::mat4 transform;

  glm::vec3 position;
  glm::vec3 forward;
  glm::vec3 up;

  float yaw;
  float pitch;
  float roll;
  float zoom;

  float fov;
  float nearPlane;
  float farPlane;

public:
  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix(float aspect, bool perspective = true);
  void SetPosition(const glm::vec3 &newPosition);
  void SetOrientation(float yaw, float pitch);
  void LookAt(const glm::vec3 &target);

  void SetZoom(float newZoom);
  void SetFov(float fov);
  void SetNearPlane(float nearPlane);
  void SetFarPlane(float farPlane);

  float GetZoom() const;
  glm::vec3 GetUp() const {
    return glm::normalize(glm::cross(GetRight(), GetForward()));
  }
  glm::vec3 GetForward() const { return glm::normalize(forward); }
  glm::vec3 GetRight() const { return glm::normalize(glm::cross(forward, up)); }
  glm::vec3 GetPosition() const { return position; }

private:
  void UpdateCameraVectors();
};
