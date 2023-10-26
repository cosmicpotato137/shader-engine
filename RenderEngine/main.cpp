#include "core.h"

#include <iostream>

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
    ptr<ComputeShader> conwayShader;
    ptr<RenderTexture> renderTarget;

    // event vars
    bool isDragging = false;
    glm::vec2 lastMousePos = glm::vec2(0, 0);
    
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
        postShader->SetUniform("textureSize", GetWindowSize());

        // compute shaders
        conwayShader = std::make_shared<ComputeShader>("conway");
        conwayShader->Init(SHADER_DIR + "/compute/conway.compute");

        // materials
        ptr<Shader> defaultShader;
        defaultShader = ren.LoadShader("defaultShader", SHADER_DIR + "/default.shader");

        ptr<Shader> testShader;
        testShader = ren.LoadShader("test", SHADER_DIR + "/fun/test.shader");

        ptr<Shader> texShader;
        texShader = ren.LoadShader("texture", SHADER_DIR + "/texture.shader");

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
        ren.PushObject(object);

        ptr<RenderObject> object2 = std::make_shared<RenderObject>(
            std::make_shared<Transform>(
                glm::vec3(0, -5, 0),
                glm::vec3(-90, 0, 0),
                glm::vec3(5)
            ),
            quad,
            std::make_shared<Material>("default", texShader, tex)
        );
        ren.PushObject(object2);

        // pre update rendering
        renderTarget = std::make_shared<RenderTexture>();
        renderTarget->Init(screenWidth, screenHeight);
        ren.SetRenderTarget(renderTarget);

        // set work groups to width and height of texture
        conwayShader->SetWorkGroups(screenWidth, screenHeight, 1);

        renderTarget->GetTexture()->BindCompute();
        renderTarget->Bind();
        conwayShader->SetUniform("color", { 1, 0, 0, 1 });
        conwayShader->Use();

        renderTarget->GetTexture()->SaveToImage(RESOURCE_DIR + "/textures/test.png");
        // apply render target to render the compute shader to the whole screen

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

        // alternative render step
        // 
        // regular render step
        //ren.Render();
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
