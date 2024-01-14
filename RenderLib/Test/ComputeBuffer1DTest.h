#pragma once
#include "gtest/gtest.h"
#include "ComputeBuffer1D.h"
#include "Application.h"

class ComputeBuffer1DTest : public ::testing::Test {
protected:
  void SetUp() override {
    app = Application::GetInstance();
    app->Init("Test", 100, 100);
    buffer = std::make_shared<ComputeBuffer1D<int>>();
  }

  void TearDown() override { Application::DestroyInstance(); }

  void TestConstructor() {
    EXPECT_EQ(buffer->GetSize(), 0);
    EXPECT_EQ(buffer->GetBufferID(), 0);
  }

  void TestInitFromVector() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    buffer->InitFromVector(data);
    EXPECT_EQ(buffer->GetSize(), data.size());
    for (int i = 0; i < data.size(); i++) {
      EXPECT_EQ(buffer->GetElement(i), data[i]);
    }
  }

  void TestPushBack() {
    int element = 6;
    buffer->PushBack(element);
    EXPECT_EQ(buffer->GetSize(), 1);
    EXPECT_EQ(buffer->GetElement(0), element);
  }

  void TestCleanup() {
    buffer->Cleanup();
    EXPECT_EQ(buffer->GetSize(), 0);
    EXPECT_EQ(buffer->GetBufferID(), 0);
  }

  Application *app;
  std::shared_ptr<ComputeBuffer1D<int>> buffer;
};

TEST_F(ComputeBuffer1DTest, Constructor) { TestConstructor(); }
TEST_F(ComputeBuffer1DTest, InitFromVector) { TestInitFromVector(); }
TEST_F(ComputeBuffer1DTest, PushBack) { TestPushBack(); }
TEST_F(ComputeBuffer1DTest, Cleanup) { TestCleanup(); }