#pragma once

#include <format>
#include "gtest/gtest.h"
#include "Renderer/ComputeShader.h"

class ComputeShaderTest : public ::testing::Test {
protected:
  void SetUp() override {
    shader = std::make_shared<ComputeShader>("TestComputeShader");
  }

  void TearDown() override { shader->Cleanup(); }

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

  void TestShaderSerialization() {
    // Init shader and assert color uniform exists
    ASSERT_TRUE(
        shader->Init(std::format("{}/compute/test-color.compute", SHADER_DIR)));
    ASSERT_TRUE(shader->HasUniform("color"));

    // Set color to white and save shader
    shader->SetUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));
    auto path = std::filesystem::current_path() / "bin" / "Debug" / "data" /
                "test_shader.dat";
    Serial::Save(*shader, path.string());

    // Assert color is black
    shader->SetUniform("color", glm::vec3(0.0f, 0.0f, 0.0f));
    ASSERT_EQ(
        shader->GetUniform("color")->GetValue<glm::vec3>(),
        glm::vec3(0.0f, 0.0f, 0.0f));

    // Load shader and assert color is white
    auto loaded_shader = Serial::Load<Shader>(path.string());
    ASSERT_EQ(
        loaded_shader->GetUniform("color")->GetValue<glm::vec3>(),
        glm::vec3(1.0f, 1.0f, 1.0f));
  }

  std::shared_ptr<ComputeShader> shader;
};

TEST_F(ComputeShaderTest, SetWorkGroups) { TestSetWorkGroups(); }
TEST_F(ComputeShaderTest, InitFromSource) { TestInit(); }
TEST_F(ComputeShaderTest, InitFromSourceFail) { TestInitFail(); }
TEST_F(ComputeShaderTest, Use) { TestUse(); }
TEST_F(ComputeShaderTest, ShaderSerialization) { TestShaderSerialization(); }