#pragma once
#include "gtest/gtest.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderTexture.h"
#include "Renderer/Mesh.h"

#include <format>

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

  void TestSerialization() {
    shader->Init(std::format("{}/hello.shader", SHADER_DIR));

    std::stringstream ss;
    ss << *shader;
    Shader deserialized;
    ss >> deserialized;
    EXPECT_EQ(shader->name, deserialized.name);
    EXPECT_EQ(shader->GetFilePath(), deserialized.GetFilePath());
    EXPECT_EQ(shader->uniforms.size(), deserialized.uniforms.size());

    for (auto &uniform : shader->uniforms) {
      EXPECT_TRUE(deserialized.HasUniform(uniform.first));
      EXPECT_EQ(
          uniform.second->GetType(),
          deserialized.GetUniform(uniform.first)->GetType());
      EXPECT_EQ(
          uniform.second->GetValue(),
          deserialized.GetUniform(uniform.first)->GetValue());
    }
  }

  ptr<RenderTexture> renderTexture;
  ptr<Shader> shader;
};

TEST_F(ShaderTest, Init) { TestInit(); }
TEST_F(ShaderTest, InitFail) { TestInitFail(); }
TEST_F(ShaderTest, HasUniform) { TestHasUniform(); }
TEST_F(ShaderTest, SetUniform) { TestSetUniform(); }
TEST_F(ShaderTest, Serialization) { TestSerialization(); }