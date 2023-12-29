#include "Application.h"
#include "gtest/gtest.h"

class TestLayer : public ApplicationLayer {
public:
  TestLayer() : ApplicationLayer("TestLayer") {}
};

class ApplicationTest : public ::testing::Test {
protected:
  void SetUp() override { app = Application::GetInstance(); }

  void TearDown() override { Application::DestroyInstance(); }

  void TestInit() { ASSERT_TRUE(app->Init("Test Window")); }

  void TestCleanup() {
    app->Init("Test Window");
    app->Cleanup();

    // Check that all the resources have been freed
    ASSERT_EQ(app->window, nullptr);
    ASSERT_EQ(app->layers.size(), 0);
    ASSERT_EQ(app->uiLayer, nullptr);
    ASSERT_EQ(app->initialized, false);
  }

  // make sure there is only one instance of the application
  void TestSingleton() {
    Application *app2 = Application::GetInstance();
    ASSERT_EQ(app, app2);
  }

  void LayerPushAndPop() {
    // Create a new layer
    ptr<TestLayer> layer = std::make_shared<TestLayer>();

    // Push the layer
    app->PushLayer(layer);

    // Check that the layer was added
    EXPECT_EQ(app->layers.back(), layer);

    ptr<ApplicationLayer> layer2 = app->layers.back();

    // Pop the layer
    app->PopLayer();

    // Check that the layer was removed
    EXPECT_TRUE(app->layers.empty());
  }

  // Check that the event was not handled correctly
  void TestEventHandling() {
    app->Init("Test Window");

    MouseButtonEvent e;
    e.handled = false;
    e.action = 1;
    e.button = 0;

    // event will go through imgui dockspace
    ASSERT_FALSE(app->OnEvent(e));
  }

  Application *app;
};

TEST_F(ApplicationTest, Init) { TestInit(); }
TEST_F(ApplicationTest, Cleanup) { TestCleanup(); }
TEST_F(ApplicationTest, Singleton) { TestSingleton(); }
TEST_F(ApplicationTest, LayerPushAndPop) { LayerPushAndPop(); }
TEST_F(ApplicationTest, EventHandling) { TestEventHandling(); }
