#pragma once
#include <format>
#include "gtest/gtest.h"
#include "ComputeShader.h"
#include "Application.h"

class ComputeShaderTest : public ::testing::Test {
protected:
  void SetUp() override {
    app = Application::GetInstance();
    app->Init("Test", 100, 100);
    shader = std::make_shared<ComputeShader>("TestComputeShader");
  }

  void TearDown() override {
    shader->Cleanup();
    Application::DestroyInstance();
  }

  void TestSetWorkGroups() {
    int x = 10, y = 20, z = 30;
    shader->SetWorkGroups(x, y, z);
    // Assuming GetWorkGroupsX, GetWorkGroupsY, and GetWorkGroupsZ are getter
    // methods
    EXPECT_EQ(shader->x, x);
    EXPECT_EQ(shader->y, y);
    EXPECT_EQ(shader->z, z);
  }

  void TestInit() {
    std::string source = std::string(SHADER_DIR) + "/compute/hello.compute";
    EXPECT_TRUE(shader->Init(source));
    EXPECT_NE(shader->GetProgramID(), 0);
  }

  void TestInitFail() {
    std::string source = "";
    EXPECT_FALSE(shader->Init(source));
    EXPECT_EQ(shader->GetProgramID(), 0);
  }

  void TestUse() {
    shader->Init(std::format("{}/compute/hello.compute", SHADER_DIR));
    shader->Use();
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
  }

  std::shared_ptr<ComputeShader> shader;
  Application *app;
};

TEST_F(ComputeShaderTest, SetWorkGroups) { TestSetWorkGroups(); }
TEST_F(ComputeShaderTest, InitFromSource) { TestInit(); }
TEST_F(ComputeShaderTest, InitFromSourceFail) { TestInitFail(); }
TEST_F(ComputeShaderTest, Use) { TestUse(); }