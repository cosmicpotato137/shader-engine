#include "re_core.h"

// layers
#include "RenderLayer.h"

class MyApp : public Application {
    void Start() override {
        PushLayer(std::make_shared<ShaderLayer>(this));
    }

    void Update(double dt) override {

    }
};

int main() {
    MyApp app;
    

    if (app.Init("Shader Ideas"))
    {
        app.Run();
        app.Cleanup();
    }
    else
    {
        Console::Error("Unable to run the app");
    }
}
