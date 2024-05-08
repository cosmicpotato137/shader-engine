#pragma once

#include <format>
#include "gtest/gtest.h"
#include "Renderer/ComputeShader.h"

class ComputeShaderTest : public ::testing::Test {
protected:
  void SetUp() override {
    computeShader = std::make_shared<ComputeShader>("TestComputeShader");
  }

  void TearDown() override { computeShader->Cleanup(); }

  void TestSetWorkGroups() {
    int x = 10, y = 20, z = 30;
    computeShader->SetWorkGroups(x, y, z);
    // Assuming GetWorkGroupsX, GetWorkGroupsY, and GetWorkGroupsZ are getter
    // methods
    EXPECT_EQ(computeShader->x, x);
    EXPECT_EQ(computeShader->y, y);
    EXPECT_EQ(computeShader->z, z);
  }

  void TestInit() {
    std::string source = std::string(SHADER_DIR) + "/compute/hello.compute";
    EXPECT_TRUE(computeShader->Init(source));
    EXPECT_NE(computeShader->GetProgramID(), 0);
  }

  void TestInitFail() {
    std::string source = "";
    EXPECT_FALSE(computeShader->Init(source));
    EXPECT_EQ(computeShader->GetProgramID(), 0);
  }

  void TestUse() {
    computeShader->Init(std::format("{}/compute/hello.compute", SHADER_DIR));
    computeShader->Use();
    EXPECT_EQ(glGetError(), GL_NO_ERROR);
  }

  void TestShaderSerialization() {
    // Init shader and assert color uniform exists
    ASSERT_TRUE(
        computeShader->Init(std::format("{}/compute/test-color.compute", SHADER_DIR)));
    ASSERT_TRUE(computeShader->HasUniform("color"));

    // Set color to white and save shader
    computeShader->SetUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));
    auto path = std::filesystem::current_path() / "bin" / "Debug" / "data" /
                "test_shader.dat";
    Serial::Save(computeShader, path.string());

    // Assert color is black
    computeShader->SetUniform("color", glm::vec3(0.0f, 0.0f, 0.0f));
    ASSERT_EQ(
        computeShader->GetUniform("color")->GetValue<glm::vec3>(),
        glm::vec3(0.0f, 0.0f, 0.0f));

    // Load shader and assert color is white
    computeShader = Serial::Load<std::shared_ptr<ComputeShader>>(path.string());
    ASSERT_EQ(
        computeShader->GetUniform("color")->GetValue<glm::vec3>(),
        glm::vec3(1.0f, 1.0f, 1.0f));
  }

  std::shared_ptr<ComputeShader> computeShader;
};

TEST_F(ComputeShaderTest, SetWorkGroups) { TestSetWorkGroups(); }
TEST_F(ComputeShaderTest, InitFromSource) { TestInit(); }
TEST_F(ComputeShaderTest, InitFromSourceFail) { TestInitFail(); }
TEST_F(ComputeShaderTest, Use) { TestUse(); }
TEST_F(ComputeShaderTest, ShaderSerialization) { TestShaderSerialization(); }