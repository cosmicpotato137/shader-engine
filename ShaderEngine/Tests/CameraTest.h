#pragma once
#include "gtest/gtest.h"
#include "Renderer/Camera.h"

class CameraTest : public ::testing::Test {
protected:
  void SetUp() override { camera = std::make_shared<Camera>(); }

  void TearDown() override {}

  void TestConstructor() {
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    Camera camera(position, up, 0, 0);

    EXPECT_EQ(camera.GetPosition(), position);
    EXPECT_EQ(camera.GetUp(), up);
    EXPECT_EQ(camera.GetYaw(), 0);
    EXPECT_EQ(camera.GetPitch(), 0);
  }

  void TestSetPosition() {
    glm::vec3 newPosition(2.0f, 3.0f, 4.0f);
    camera->SetPosition(newPosition);
    EXPECT_EQ(camera->GetPosition(), newPosition);
  }

  void TestSetOrientation() {
    float newYaw = 60.0f;
    float newPitch = 40.0f;
    camera->SetOrientation(newYaw, newPitch);
    EXPECT_EQ(camera->GetYaw(), newYaw);
    EXPECT_EQ(camera->GetPitch(), newPitch);
  }

  void TestLookAt() {
    glm::vec3 target(3.0f, 4.0f, 5.0f);
    camera->LookAt(target);

    // expected pitch and yaw
    glm::vec3 direction = glm::normalize(target - camera->GetPosition());
    float expected_pitch = glm::degrees(asin(direction.y));
    float expected_yaw = glm::degrees(atan2(direction.z, direction.x));

    EXPECT_FLOAT_EQ(camera->GetPitch(), expected_pitch);
    EXPECT_FLOAT_EQ(camera->GetYaw(), expected_yaw);
  }

  void TestSetZoom() {
    float newZoom = 60.0f;
    camera->SetZoom(newZoom);
    EXPECT_FLOAT_EQ(camera->GetZoom(), newZoom);
  }

  std::shared_ptr<Camera> camera;
};

TEST_F(CameraTest, Constructor) { TestConstructor(); }
TEST_F(CameraTest, SetPosition) { TestSetPosition(); }
TEST_F(CameraTest, SetOrientation) { TestSetOrientation(); }
TEST_F(CameraTest, LookAt) { TestLookAt(); }
TEST_F(CameraTest, SetZoom) { TestSetZoom(); }