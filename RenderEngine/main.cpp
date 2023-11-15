#include "core.h"

#include <iostream>
#include <cmath>

#include "Renderer.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Log.h"
#include "Application.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "ApplicationLayer.h"

// layers
#include "RenderLayer.h"

#define SHADER_DIR std::string("C:/Users/imacs/Documents/Code/Shader-Ideas/shaders")
#define RESOURCE_DIR std::string("C:/Users/imacs/Documents/Code/Shader-Ideas/res")
#define TEXTURE_DIR RESOURCE_DIR + std::string("/textures")

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
