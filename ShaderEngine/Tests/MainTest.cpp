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
#include "SerializationTest.h"

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

// Check if running from default build directory
#ifdef SHADER_ENGINE_BUILD_DIR
  props.WorkingDirectory = std::string(SHADER_ENGINE_BUILD_DIR);
#else
  props.WorkingDirectory = Application::GetApplicationPath();
#endif

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
// #include "testtest.h"

// int main() {
//   {
//     AbstractPoint *point(new Point(7.4));

//     std::ofstream outputStream(path + "test.txt");
//     boost::archive::text_oarchive outputArchive(outputStream);
//     outputArchive << point;
//     outputStream.close();
//     delete point;
//   }

//   {
//     AbstractPoint *pointRead;

//     std::ifstream inputStream(path + "test.txt");
//     boost::archive::text_iarchive inputArchive(inputStream);
//     inputArchive >> pointRead;

//     if (auto p = static_cast<Point *>(pointRead))
//       std::cout << p->mData << std::endl;
//     delete pointRead;
//   }

//   return 0;
// }
