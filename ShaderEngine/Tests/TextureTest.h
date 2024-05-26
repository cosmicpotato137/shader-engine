#pragma once
#include <gtest/gtest.h>
#include "Renderer/RenderTexture.h"

class TextureTest : public ::testing::Test {
  std::shared_ptr<Texture> texture;

protected:
  // SetUp will be called before each test case
  void SetUp() override { texture = std::make_shared<Texture>(); }

  // TearDown will be called after each test case
  void TearDown() override { texture->Cleanup(); }

  void InitTest() {
    int width = 10, height = 20;
    EXPECT_TRUE(texture->Init(width, height));
    EXPECT_NE(texture->GetTextureID(), 0);
    EXPECT_EQ(texture->GetWidth(), width);
    EXPECT_EQ(texture->GetHeight(), height);
  }

  void InitFailTest() {
    int width = 0, height = 0;
    EXPECT_FALSE(texture->Init(width, height));
    EXPECT_EQ(texture->GetWidth(), width);
    EXPECT_EQ(texture->GetHeight(), height);
  }

  void InitFromImageTest() {
    std::string filePath = Application::GetInstance()->GetWorkingDirectory() +
                           "/res/textures/test.png";
    EXPECT_TRUE(texture->Init(filePath));
    EXPECT_NE(texture->GetTextureID(), 0);
    EXPECT_NE(texture->GetWidth(), 0);
    EXPECT_NE(texture->GetHeight(), 0);
  }

  void InitFromImageFailTest() {
    std::string filePath = "";
    EXPECT_FALSE(texture->Init(filePath));
    EXPECT_EQ(texture->GetWidth(), 0);
    EXPECT_EQ(texture->GetHeight(), 0);
  }

  void ReadWritePixelTest() {
    int x = 0, y = 0;
    unsigned char r = 255, g = 0, b = 0, a = 255;
    texture->Init(1, 1);
    texture->WritePixel(x, y, r, g, b, a);
    EXPECT_EQ(texture->GetPixel(x, y).r, r);
    EXPECT_EQ(texture->GetPixel(x, y).g, g);
    EXPECT_EQ(texture->GetPixel(x, y).b, b);
    EXPECT_EQ(texture->GetPixel(x, y).a, a);
  }

  void ReadWritePixelFailTest() {
    int x = 0, y = 0;
    unsigned char r = 255, g = 0, b = 0, a = 255;
    texture->Init(0, 0);
    try {
      texture->WritePixel(x, y, r, g, b, a);
      FAIL();
    } catch (std::exception &e) {
      EXPECT_EQ(e.what(), std::string("[EXCEPTION] Texture not initialized"));
    }
  }
};

TEST_F(TextureTest, Init) { InitTest(); }
TEST_F(TextureTest, InitFail) { InitFailTest(); }
TEST_F(TextureTest, InitFromImage) { InitFromImageTest(); }
TEST_F(TextureTest, InitFromImageFail) { InitFromImageFailTest(); }
TEST_F(TextureTest, ReadWritePixel) { ReadWritePixelTest(); }
TEST_F(TextureTest, ReadWritePixelFail) { ReadWritePixelFailTest(); }
