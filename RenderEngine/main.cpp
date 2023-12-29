#include "re_core.h"

// Layers
#include "RenderLayer.h"
#include "SimulationLayer.h"

int main() {
  Application *app = Application::GetInstance();

  if (app->Init("Shader Ideas")) {
    // App.PushLayer(std::make_shared<RenderLayer>());
    app->PushLayer(std::make_shared<SimulationLayer>());
    app->Run();

    // Clean up all resources
    Application::DestroyInstance();
  } else {
    Console::Error("Unable to run the app");
  }
}
