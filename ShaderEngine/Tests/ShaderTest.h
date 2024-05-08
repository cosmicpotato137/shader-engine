#pragma once
#include "gtest/gtest.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderTexture.h"
#include "Renderer/Mesh.h"

#include <format>
#include <filesystem>

class ShaderTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    renderTexture = std::make_shared<RenderTexture>();
    renderTexture->Init(100, 100, false);
    shader = std::make_shared<Shader>("Test Shader");
  }

  virtual void TearDown() {
    shader->Cleanup();
    renderTexture->Cleanup();
  }

  void TestInit() {
    ASSERT_TRUE(shader->Init(
        std::format("{}/postProcessing/defaultPost.shader", SHADER_DIR)));
  }

  void TestInitFail() { ASSERT_FALSE(shader->Init("badDir")); }

  void TestHasUniform() {
    ASSERT_TRUE(shader->Init(std::format("{}/hello.shader", SHADER_DIR)));
    ASSERT_TRUE(shader->HasUniform("color"));

    ptr<Uniform> uniform = shader->GetUniform("color");
    ASSERT_EQ(uniform->GetType(), UniformType::Col3);
    ASSERT_EQ(uniform->GetHide(), false);
    ASSERT_FALSE(shader->HasUniform("number"));
  }

  void TestSetUniform() {
    ASSERT_TRUE(shader->Init(std::format("{}/hello.shader", SHADER_DIR)));
    ASSERT_TRUE(shader->HasUniform("color"));
    shader->SetUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));

    ptr<Mesh> mesh = Mesh::Quad();

    renderTexture->BeginRender();
    shader->Use();
    mesh->Draw();
    renderTexture->EndRender();

    ptr<Texture> texture = renderTexture->GetTexture();
    ASSERT_EQ(texture->GetPixel(0, 0), glm::vec4(255, 0, 0, 255));
  }

  void TestShaderSerialization() {
    // Init shader and assert color uniform exists
    ASSERT_TRUE(shader->Init(std::format("{}/hello.shader", SHADER_DIR)));
    ASSERT_TRUE(shader->HasUniform("color"));

    // Set color to white and save shader
    shader->SetUniform("color", glm::vec3(1.0f, 1.0f, 1.0f));
    auto path = std::filesystem::current_path() / "bin" / "Debug" / "data" /
                "test_shader.dat";
    Serial::Save(shader, path.string());

    // Assert color is black
    shader->SetUniform("color", glm::vec3(0.0f, 0.0f, 0.0f));
    ASSERT_EQ(
        shader->GetUniform("color")->GetValue<glm::vec3>(),
        glm::vec3(0.0f, 0.0f, 0.0f));

    // Load shader and assert color is white
    shader = Serial::Load<std::shared_ptr<Shader>>(path.string());
    ASSERT_EQ(
        shader->GetUniform("color")->GetValue<glm::vec3>(),
        glm::vec3(1.0f, 1.0f, 1.0f));
  }

  ptr<RenderTexture> renderTexture;
  ptr<Shader> shader;
};

TEST_F(ShaderTest, Init) { TestInit(); }
TEST_F(ShaderTest, InitFail) { TestInitFail(); }
TEST_F(ShaderTest, HasUniform) { TestHasUniform(); }
TEST_F(ShaderTest, SetUniform) { TestSetUniform(); }
TEST_F(ShaderTest, ShaderSerialization) { TestShaderSerialization(); }