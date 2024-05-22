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

  // find the directory the application was started from
  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  std::string fullPath(buffer);
  std::filesystem::path path(fullPath);
  std::string directory = path.parent_path().string();

  // store the directory in the props
  props.WorkingDirectory = directory;

  return new ShaderSandbox(props);
}
