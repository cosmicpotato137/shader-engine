#pragma once
#include "gtest/gtest.h"
#include "Application.h"
#include "RenderTexture.h"

class RenderTextureTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    app = Application::GetInstance();
    app->Init("RenderTextureTest", 100, 100);
    renderTexture = std::make_shared<RenderTexture>();
  }

  virtual void TearDown() {
    renderTexture->Cleanup();
    Application::DestroyInstance();
  }

  void TestInit() {
    EXPECT_TRUE(renderTexture->Init(100, 100, false));
    EXPECT_EQ(100, renderTexture->GetWidth());
    EXPECT_EQ(100, renderTexture->GetHeight());
  }

  Application *app;
  ptr<RenderTexture> renderTexture;
};

TEST_F(RenderTextureTest, Init) { TestInit(); }