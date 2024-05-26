// Layers
// #include "RenderLayer.h"
#include "ShaderEngine.h"
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

class ShaderSandbox : public Application {
public:
  ShaderSandbox(const ApplicationProps &props = ApplicationProps())
      : Application(props) {}

  ~ShaderSandbox() {}

  virtual void InitLayerStack() override {
    // AddLayer(new RenderLayer());
    PushLayer(std::make_shared<EditorLayer>());
  }
};

Application *CreateApplication(const ApplicationCommandLineArgs &args) {
  ApplicationProps props;
  props.Name = "Shader Sandbox";
  props.Args = args;

  // store the directory in the props
  props.WorkingDirectory = Application::GetApplicationPath();

  return new ShaderSandbox(props);
}
