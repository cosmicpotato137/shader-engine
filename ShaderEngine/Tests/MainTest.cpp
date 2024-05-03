#include "gtest/gtest.h"
#include "Core/Application.h"
#include "ApplicationTest.h"
#include "ComputeBuffer1DTest.h"
#include "CameraTest.h"
#include "ComputeShaderTest.h"
#include "TextureTest.h"
#include "RendererTest.h"
#include "RenderTextureTest.h"
#include "ShaderTest.h"
#include "MaterialTest.h"
#include "ShaderUniformTest.h"

class ApplicationTest : public Application {
public:
  ApplicationTest(const ApplicationProps &props) : Application(props) {}

  virtual void InitLayerStack() override {}
};

Application *CreateApplication(const ApplicationCommandLineArgs &args) {
  ApplicationProps props;
  props.Name = "Test Application";
  props.Args = args;
  props.Width = 10;
  props.Height = 10;
  return new ApplicationTest(props);
}

int main(int argc, char **argv) {
  Application *app = CreateApplication({argc, argv});

  if (app->Init()) {
    // run all tests
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    Application::DestroyInstance();
    return result;
  } else {
    Console::Assert(false, "Unable to run the app");
    // return failure
    return 1;
  }
}