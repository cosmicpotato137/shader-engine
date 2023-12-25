#include "re_core.h"

// Layers
#include "RenderLayer.h"
#include "SimulationLayer.h"

int main() {
  Application app;

  if (app.Init("Shader Ideas")) {
    //app.PushLayer(std::make_shared<RenderLayer>());
    app.PushLayer(std::make_shared<SimulationLayer>());
    app.Run();
    app.Cleanup();
  } else {
    Console::Error("Unable to run the app");
  }
}
