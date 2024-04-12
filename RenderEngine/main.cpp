#include "re_core.h"

// Layers
#include "RenderLayer.h"
#include "SimulationLayer.h"
#include "OutputLayer.h"

int main() {
  Application *app = Application::GetInstance();

  if (app->Init("Shader Ideas")) {
    app->PushLayer(std::make_shared<RenderLayer>());
    // app->PushLayer(std::make_shared<SimulationLayer>());
    app->PushLayer(std::make_shared<OutputLayer>());
    app->Run();

    // Clean up all resources
    Application::DestroyInstance();
  } else {
    Console::Assert(false, "Unable to run the app");
  }
}
