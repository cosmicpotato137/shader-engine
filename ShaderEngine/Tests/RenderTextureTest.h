#pragma once
#include "gtest/gtest.h"
#include "Renderer/RenderTexture.h"

class RenderTextureTest : public ::testing::Test {
protected:
  virtual void SetUp() { renderTexture = std::make_shared<RenderTexture>(); }

  virtual void TearDown() { renderTexture->Cleanup(); }

  void TestInit() {
    EXPECT_TRUE(renderTexture->Init(100, 100, false));
    EXPECT_EQ(100, renderTexture->GetWidth());
    EXPECT_EQ(100, renderTexture->GetHeight());
  }

  ptr<RenderTexture> renderTexture;
};

TEST_F(RenderTextureTest, Init) { TestInit(); }