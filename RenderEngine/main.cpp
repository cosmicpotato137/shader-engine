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

#define SHADER_DIR std::string("C:/Users/imacs/Documents/NerdThings/Shader-Ideas/shaders")
#define RESOURCE_DIR std::string("C:/Users/imacs/Documents/NerdThings/Shader-Ideas/res")
#define TEXTURE_DIR RESOURCE_DIR + std::string("/textures")


class MyApp : public Application {

    // render vars
    Renderer ren;
    ptr<Mesh> quad;
    ptr<Mesh> box;

    ptr<Camera> cam;

    ptr<Shader> postShader;
    ptr<ComputeShader> helloShader;
    ptr<ComputeShader> conwayShader;
    ptr<ComputeShader> mandelbrotShader;
    ptr<RenderTexture> renderTarget;
    ptr<RenderTexture> swapTarget;

    // event vars
    bool isDragging = false;
    glm::vec2 lastMousePos = glm::vec2(0, 0);

    glm::vec2 centerPos = glm::vec2(0);
    float zoom = 1.0f;
    
    float resetTime = 0.0f;

protected:
    virtual void Start() override
    {
        // renderer settings
        ren.Init(window);
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

    virtual void Update(double dt) override
    {
        //ren.Render();
        //if (GetTime() - resetTime > 1)
        //{
        //    resetTime = GetTime();
        //    postShader->SetUniform("step", true);
        //}
        //postShader->SetUniform("step", false);
        //std::swap(ren.GetRenderTarget(), renderTarget);

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
        mandelbrotShader->SetUniform("center", centerPos / GetWindowSize());
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

    virtual void OnKey(int key, int scancode, int action, int mods) override {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (key == GLFW_KEY_R)
        {
            cam->SetPosition({ 0, 0, -5 });
            cam->LookAt({ 0, 0, 0 });
        }
    }

    virtual void OnMousePos(double xpos, double ypos) override {
        if (isDragging) {
            // Calculate the offset of the cursor from the last position
            float xOffset = xpos - lastMousePos.x;
            float yOffset = ypos - lastMousePos.y; // Invert Y-axis if needed

            // Update the position of the dragged object (e.g., camera, object, etc.)
            RotateCamera(xOffset, yOffset);

            centerPos.x -= xOffset * zoom * 2 * GetAspect();
            centerPos.y += yOffset * zoom * 2;

            // Update the last cursor position for the next frame
            lastMousePos = glm::vec2(xpos, ypos);
        }
    }

    virtual void OnMouseButton(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                isDragging = true;
                lastMousePos = GetCursorPosition();
            }
            else if (action == GLFW_RELEASE) {
                isDragging = false;
            }
        }
    }

    virtual void OnScroll(double xoffset, double yoffset) override {
        float len = glm::length(cam->GetPosition());
        cam->SetPosition(glm::normalize(cam->GetPosition()) * glm::clamp(len + -1.0f * (float)yoffset, 1.0f, 100.0f));
        zoom += yoffset * zoom;
        zoom = std::max(zoom, 0.000001f);
        zoom = std::min(zoom, 3.0f);
    }

    virtual void OnWindowResize(int width, int height)
    {
        Application::OnWindowResize(width, height);
        // resize textures
        renderTarget->Cleanup();
        renderTarget->Init(screenWidth, screenHeight);
        swapTarget->Cleanup();
        swapTarget->Init(screenWidth, screenHeight);

        // update shader work groups
        conwayShader->SetWorkGroups(std::ceil(screenWidth / 8.0), std::ceil(screenHeight / 8.0), 1);
        mandelbrotShader->SetWorkGroups(std::ceil(screenWidth / 8.0), std::ceil(screenHeight / 8.0), 1);
        helloShader->SetWorkGroups(std::ceil(screenWidth / 8.0), std::ceil(screenHeight / 8.0), 1);
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
