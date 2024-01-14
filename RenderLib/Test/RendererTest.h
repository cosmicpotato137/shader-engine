#pragma once
#include <gtest/gtest.h>
#include "Application.h"
#include "Renderer.h"

class RendererTest : public ::testing::Test {
  std::shared_ptr<Renderer> renderer;
  Application *app;

public:
  void SetUp() override {
    app = Application::GetInstance();
    app->Init("Test", 100, 100);
    renderer = std::make_shared<Renderer>();
  }

  void TearDown() override {
    renderer->Cleanup();
    Application::DestroyInstance();
  }

  void TestInit() {
    glm::vec2 size = Application::GetInstance()->GetWindowSize();
    ASSERT_TRUE(renderer->Init(size.x, size.y));
    EXPECT_NE(renderer->GetRenderTarget()->GetFramebufferID(), 0);
    EXPECT_NE(renderer->GetSwapTarget()->GetFramebufferID(), 0);
    EXPECT_EQ(renderer->GetSize(), glm::vec2(size.x, size.y));
  }

  void TestRender() {
    renderer->Init(100, 100);

    RenderStep step([&]() {
      Renderer::SetClearColor(1, 0, 0);
      renderer->Clear();
    });

    renderer->PushRenderStep(step);

    // test initial render
    renderer->Render(false);
    ptr<Texture> swap = renderer->GetSwapTarget()->GetTexture();
    ptr<Texture> tex = renderer->GetRenderTarget()->GetTexture();
    glm::vec4 col = swap->GetPixel(0, 0);
    EXPECT_EQ(col, glm::vec4(255, 0, 0, 255));
    EXPECT_EQ(tex->GetPixel(0, 0).x, 0);

    // test swap
    renderer->Render();
    EXPECT_EQ(tex->GetPixel(0, 0), glm::vec4(255, 0, 0, 255));
    EXPECT_EQ(swap->GetPixel(0, 0), glm::vec4(255, 0, 0, 255));
  }
};

TEST_F(RendererTest, Init) { TestInit(); }
TEST_F(RendererTest, Render) { TestRender(); }
