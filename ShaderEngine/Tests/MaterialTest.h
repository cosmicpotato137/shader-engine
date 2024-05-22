#pragma once

#include "gtest/gtest.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/RenderTexture.h"

#include <format>

class MaterialTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    renderTexture = std::make_shared<RenderTexture>();
    renderTexture->Init(100, 100, false);

    ptr<Texture> tex = std::make_shared<Texture>();
    tex->Init(1, 1);
    tex->WritePixel(0, 0, 255, 0, 0);  // init to red

    ptr<Shader> shader = std::make_shared<Shader>("Test Shader");
    shader->Init(
        std::format("{}/postProcessing/defaultPost.shader", SHADER_DIR));

    material = std::make_shared<Material>("test material", shader, tex);
  }

  virtual void TearDown() {
    renderTexture->Cleanup();
    material->Cleanup();
  }

  void TestRender() {
    ptr<Mesh> mesh = Mesh::Quad();

    renderTexture->BeginRender();
    material->Bind();
    mesh->Draw();
    renderTexture->EndRender();

    EXPECT_EQ(255, renderTexture->GetTexture()->GetPixel(0, 0).r);
  }

  ptr<Material> material;
  ptr<RenderTexture> renderTexture;
};

TEST_F(MaterialTest, Render) { TestRender(); }