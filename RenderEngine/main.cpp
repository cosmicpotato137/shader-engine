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

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#define SHADER_DIR std::string("C:/Users/imacs/Documents/NerdThings/Shader-Ideas/shaders")
#define RESOURCE_DIR std::string("C:/Users/imacs/Documents/NerdThings/Shader-Ideas/res")
#define TEXTURE_DIR RESOURCE_DIR + std::string("/textures")

class ImGUILayer : public ApplicationLayer {
    Application* app;
public:
    ImGUILayer(Application* app) : ApplicationLayer("imgui layer"), app(app) {
        // init imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // set imgui style
        ImGui::StyleColorsDark();

        // Initialize ImGui for GLFW
        ImGui_ImplGlfw_InitForOpenGL(app->GetWindow(), true);

        // Initialize ImGui for OpenGL
        ImGui_ImplOpenGL3_Init("#version 460 core");
    }

    void Update(double dt) {

        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create an ImGui window
        ImGui::Begin("ImGui Window");

        // Add ImGui widgets, e.g., buttons, sliders, etc.
        ImGui::Text("Hello, ImGui!");
        if (ImGui::Button("Click Me")) {
            Console::Log("Hello world!");
        }

        // End the ImGui window
        ImGui::End();
    }

    void Render() override {

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void HandleEvent(MouseButtonEvent& e) override
    {
        e.handled = ImGui::GetIO().WantCaptureMouse;
    }

    void HandleEvent(WindowResizeEvent& e) override
    {
        
    }
};

class ShaderLayer : public ApplicationLayer {
    Application* app;

    // render vars
    Renderer ren;
    ptr<RenderTexture> renderTarget;
    ptr<RenderTexture> swapTarget;
    ptr<Camera> cam;

    // meshes
    ptr<Mesh> quad;
    ptr<Mesh> box;

    // shaders
    ptr<Shader> postShader;
    ptr<ComputeShader> helloShader;
    ptr<ComputeShader> conwayShader;
    ptr<ComputeShader> mandelbrotShader;

    // event vars
    bool isDragging = false;
    glm::vec2 lastMousePos = glm::vec2(0, 0);

    glm::vec2 centerPos = glm::vec2(0);
    float zoom = 1.0f;

    float resetTime = 0.0f;

public:
    ShaderLayer(Application* app) : ApplicationLayer("main render layer"), app(app) {
        // renderer settings
        ren.Init(app->GetWindow());
        cam = ren.GetMainCamera();

        ren.SetClearColor(.3, .3, .3);
        ren.Clear();

        // post proceccing
        postShader = std::make_shared<Shader>("postShader");
        postShader->Init(SHADER_DIR + "/postProcessing/defaultPost.shader");
        ren.SetPostProcess(std::make_shared<Material>(
            "defaultPost",
            postShader
        ));

        // compute shaders
        helloShader = std::make_shared<ComputeShader>("hello");
        helloShader->Init(SHADER_DIR + "/compute/hello.compute");

        conwayShader = std::make_shared<ComputeShader>("conway");
        conwayShader->Init(SHADER_DIR + "/compute/conway.compute");

        mandelbrotShader = std::make_shared<ComputeShader>("mandelbrot");
        mandelbrotShader->Init(SHADER_DIR + "/compute/mandelbrot.compute");

        // materials
        ptr<Shader> defaultShader;
        defaultShader = ren.LoadShader("defaultShader", SHADER_DIR + "/default.shader");

        ptr<Shader> testShader;
        testShader = ren.LoadShader("test", SHADER_DIR + "/fun/test.shader");

        ptr<Shader> texShader;
        texShader = ren.LoadShader("texture", SHADER_DIR + "/texture.shader");

        ptr<Shader> noiseShader = ren.LoadShader("noise", SHADER_DIR + "/fun/noise.shader");
        ptr<Material> noiseMat = std::make_shared<Material>(
            "noiseMat", noiseShader, ren.GetRenderTarget()->GetTexture()
        );

        // textures
        ptr<Texture> tex;
        tex = std::make_shared<Texture>();
        tex->Init(TEXTURE_DIR + "/checkers-2.png");

        // objects
        quad = Mesh::Quad();
        box = Mesh::Cube();

        ptr<RenderObject> object = std::make_shared<RenderObject>(
            std::make_shared<Transform>(
                glm::vec3(0, 0, 0),
                glm::vec3(45, 45, 45),
                glm::vec3(2)
            ),
            Mesh::Sphere(50, 70),
            std::make_shared<Material>("default", testShader, tex)
        );
        //ren.PushObject(object);

        ptr<RenderObject> object2 = std::make_shared<RenderObject>(
            std::make_shared<Transform>(
                glm::vec3(0, -5, 0),
                glm::vec3(-90, 0, 0),
                glm::vec3(5)
            ),
            quad,
            std::make_shared<Material>("default", texShader, tex)
        );
        //ren.PushObject(object2);

        ptr<RenderObject> noiseQuad = std::make_shared<RenderObject>(
            std::make_shared<Transform>(),
            quad,
            noiseMat
        );
        ren.PushObject(noiseQuad);

        glm::vec2 screenSize = app->GetWindowSize();
        int screenWidth = screenSize.x;
        int screenHeight = screenSize.y;

        // assign the render target texture to the compute shader's buffer
        renderTarget = std::make_shared<RenderTexture>();
        renderTarget->Init(screenWidth, screenHeight);
        ren.SetRenderTarget(renderTarget);

        // make render target to swap into
        swapTarget = std::make_shared<RenderTexture>();
        swapTarget->Init(screenWidth, screenHeight);

        // set work groups to width and height of texture
        conwayShader->SetWorkGroups(std::ceil(screenWidth / 8.0), std::ceil(screenHeight / 8.0), 1);
        mandelbrotShader->SetWorkGroups(std::ceil(screenWidth / 8.0), std::ceil(screenHeight / 8.0), 1);
        helloShader->SetWorkGroups(std::ceil(screenWidth / 8.0), std::ceil(screenHeight / 8.0), 1);

        // render objects in scene
        ren.Render();
    }

    virtual void Render() override {

        // Your OpenGL rendering code goes here
        // hello world compute shader
        //renderTarget->GetTexture()->BindCompute(0);
        //helloShader->Use();

        // render conway's game of life
        //renderTarget->GetTexture()->BindCompute(0);
        //swapTarget->GetTexture()->BindCompute(1);
        //conwayShader->Use();
        //renderTarget.swap(swapTarget);

        // render the mandelbrot fractal
        renderTarget->GetTexture()->BindCompute(0);
        mandelbrotShader->SetUniform("center", centerPos / app->GetWindowSize());
        mandelbrotShader->SetUniform("scale", zoom);
        mandelbrotShader->SetUniform("max_iterations", unsigned int(30 / std::pow(zoom, 1.0 / 3.0)));
        mandelbrotShader->Use();

        ren.PostProcess();
    }

    void RotateCamera(float xOffset, float yOffset) {
        glm::vec3 pos = cam->GetPosition();
        float len = glm::length(pos);

        // find the new position for the camera
        float rotspd = .05f * len;
        glm::vec3 rightOffset = cam->GetRight() * -xOffset * rotspd;
        glm::vec3 upOffset = cam->GetUp() * yOffset * rotspd;
        glm::vec3 newPos = glm::normalize(pos + upOffset + rightOffset) * len;

        // ensure there isn't flipping at the poles
        float d = glm::dot(glm::normalize(newPos), glm::vec3(0, 1, 0));
        if (1 - abs(d) > 0.01f)
        {
            cam->SetPosition(newPos);
            cam->LookAt({ 0, 0, 0 });
        }
    }

    virtual void HandleEvent(KeyboardEvent& keyEvent) override {
        int key = keyEvent.key;
        int scancode = keyEvent.scancode;
        int action = keyEvent.action;
        int mods = keyEvent.mods;

        if (key == GLFW_KEY_ESCAPE)
        {
            app->SetWindowShouldClose(true);
            keyEvent.handled = true;
        }
        if (key == GLFW_KEY_R)
        {
            cam->SetPosition({ 0, 0, -5 });
            cam->LookAt({ 0, 0, 0 });
            keyEvent.handled = true;
        }

    }

    virtual void HandleEvent(CursorMovedEvent& cursorMoved) override {
        float xpos = cursorMoved.xPos;
        float ypos = cursorMoved.yPos;

        if (isDragging) {
            // Calculate the offset of the cursor from the last position
            float xOffset = xpos - lastMousePos.x;
            float yOffset = ypos - lastMousePos.y; // Invert Y-axis if needed

            // Update the position of the dragged object (e.g., camera, object, etc.)
            RotateCamera(xOffset, yOffset);

            centerPos.x -= xOffset * zoom * 2 * app->GetAspect();
            centerPos.y += yOffset * zoom * 2;

            // Update the last cursor position for the next frame
            lastMousePos = glm::vec2(xpos, ypos);

            cursorMoved.handled = true;
        }
    }

    virtual void HandleEvent(MouseButtonEvent& mouseButton) override {
        int button = mouseButton.button;
        int action = mouseButton.action;

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                isDragging = true;
                lastMousePos = app->GetCursorPosition();
            }
            else if (action == GLFW_RELEASE) {
                isDragging = false;
            }

            mouseButton.handled = true;
        }
    }

    virtual void HandleEvent(ScrollEvent& scrollEvent) override {
        float yoffset = scrollEvent.yOffset;

        float len = glm::length(cam->GetPosition());
        cam->SetPosition(glm::normalize(cam->GetPosition()) * glm::clamp(len + -1.0f * (float)yoffset, 1.0f, 100.0f));
        zoom += yoffset * zoom;
        zoom = std::max(zoom, 0.000001f);
        zoom = std::min(zoom, 3.0f);

        scrollEvent.handled = true;
    }

    virtual void HandleEvent(WindowResizeEvent& resize)
    {
        int width = resize.width;
        int height = resize.height;

        glm::vec2 screenSize = app->GetWindowSize();
        // resize textures
        renderTarget->Cleanup();
        renderTarget->Init(screenSize.x, screenSize.y);
        swapTarget->Cleanup();
        swapTarget->Init(screenSize.x, screenSize.y);

        // update shader work groups
        conwayShader->SetWorkGroups(std::ceil(screenSize.x / 8.0), std::ceil(screenSize.y / 8.0), 1);
        mandelbrotShader->SetWorkGroups(std::ceil(screenSize.x / 8.0), std::ceil(screenSize.y / 8.0), 1);
        helloShader->SetWorkGroups(std::ceil(screenSize.x / 8.0), std::ceil(screenSize.y / 8.0), 1);
    }
};

class MyApp : public Application {
protected:
    virtual void Start() override
    {
        PushLayer(std::make_shared<ShaderLayer>(this));
        PushLayer(std::make_shared<ImGUILayer>(this));
    }

    virtual void Update(double dt) override
    {
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
