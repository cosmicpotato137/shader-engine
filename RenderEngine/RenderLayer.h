#pragma once

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

#include "re_core.h"

class RenderLayer : public ApplicationLayer {
    Application* app;

    // render vars
    Renderer ren;
    ptr<RenderTexture> renderTarget;
    ptr<RenderTexture> swapTarget;
    ptr<Camera> cam;

    ptr<ComputeShader> shader;

    // event vars
    glm::vec2 lastMousePos = glm::vec2(0, 0);
    bool isDragging = false;

    glm::vec2 centerPos = glm::vec2(0);
    float zoom = 1.0f;

    glm::vec2 imContentSize;
    glm::vec2 renderTargetSize;

public:
    RenderLayer(Application* app) : ApplicationLayer("main render layer"), app(app) {
        // renderer settings
        ren.Init(app->GetWindow());
        cam = ren.GetMainCamera();

        ren.SetClearColor(.3, .3, .3);
        ren.Clear();

        glm::vec2 screenSize = renderTargetSize;
        int screenWidth = screenSize.x;
        int screenHeight = screenSize.y;

        // assign the render target texture to the compute shader's buffer
        renderTarget = std::make_shared<RenderTexture>();
        renderTarget->Init(screenWidth, screenHeight);
        ren.SetRenderTarget(renderTarget);

        // make render target to swap into
        swapTarget = std::make_shared<RenderTexture>();
        swapTarget->Init(screenWidth, screenHeight);

        shader = std::make_shared<ComputeShader>("shader");
        shader->Init(SHADER_DIR + "/compute/mandelbrot.compute");
    }

    virtual void Update(double dt) override {
    }

    virtual void Render() override {

        renderTarget->GetTexture()->BindCompute(0);
        shader->SetUniform("center", centerPos);
        shader->SetUniform("scale", zoom);
        shader->Use();

        renderTarget.swap(swapTarget);
    }

    virtual void ImGuiRender() override
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 3.0, 3.0 });

        ImGui::Begin("Scene");

        HandleSceneWindowResize();
        HandleSceneMouseEvent();

        ImVec2 s = ImGui::GetContentRegionAvail();
        imContentSize = { s.x, s.y };

        // render image to layer
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            (void*)ren.GetRenderTarget()->GetTexture()->GetTextureID(),
            ImVec2(pos),
            ImVec2(pos.x + imContentSize.x, pos.y + imContentSize.y),
            ImVec2(0, 1), ImVec2(1, 0)
        );

        ImGui::End();
        ImGui::PopStyleVar(3);

        ImGui::Begin("Uniforms");

        std::unordered_map<std::string, ptr<Uniform>> uniforms = shader->GetUniforms();
        for (auto iter = uniforms.begin(); iter != uniforms.end(); ++iter)
        {
            std::string name = iter->second->GetName();
            uniform_types value = iter->second->GetValue();

            if (iter->second->GetHide() || iter->second->GetType() == Error)
                continue;

            struct Visitor {
                ptr<Uniform> uniform;

                Visitor(ptr<Uniform> uniform) : uniform(uniform) {}

                void operator()(bool& value) {
                    ImGui::Checkbox(uniform->GetName().c_str(), &value);
                }
                void operator()(GLint& value) {
                    int v = value;
                    ImGui::DragInt(uniform->GetName().c_str(), &v, -INT_MAX, INT_MAX);
                    value = v;
                }
                void operator()(GLuint& value) {
                    int v = value;
                    ImGui::DragInt(uniform->GetName().c_str(), &v, 1.0f, -INT_MAX, INT_MAX);
                    value = v;
                }
                void operator()(GLfloat& value) {
                    ImGui::DragFloat(uniform->GetName().c_str(), &(float)value, .1f, -FLT_MAX, FLT_MAX);
                }
                void operator()(glm::vec2& value) {
                    ImGui::DragFloat2(uniform->GetName().c_str(), glm::value_ptr(value), .1f, -FLT_MAX, FLT_MAX);
                }
                void operator()(glm::vec3& value) {
                    if (uniform->GetType() == Col3)
                        ImGui::ColorEdit3(uniform->GetName().c_str(), glm::value_ptr(value));
                    else
                        ImGui::DragFloat3(uniform->GetName().c_str(), glm::value_ptr(value), .1f, -FLT_MAX, FLT_MAX);
                }
                void operator()(glm::vec4& value) {
                    if (uniform->GetType() == Col4)
                        ImGui::ColorEdit4(uniform->GetName().c_str(), glm::value_ptr(value));
                    else
                        ImGui::DragFloat4(uniform->GetName().c_str(), glm::value_ptr(value), .1f, -FLT_MAX, FLT_MAX);

                }
                void operator()(const glm::mat4& value) {
                }
            };

            std::visit(Visitor(iter->second), value);
            iter->second->SetValue(value);
        }

        ImGui::End();
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
        if (ImGui::GetIO().WantCaptureKeyboard)
        {
            keyEvent.handled = true;
            return;
        }

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
        if (ImGui::GetIO().WantSetMousePos)
        {
            cursorMoved.handled = true;
            return;
        }
    }

    virtual void HandleEvent(MouseButtonEvent& mouseButton) override {
        if (ImGui::GetIO().WantCaptureMouse)
        {
            mouseButton.handled = true;
            return;
        }
    }

    virtual void HandleEvent(ScrollEvent& scrollEvent) override {
        if (ImGui::GetIO().WantCaptureMouse)
        {
            scrollEvent.handled = true;
            return;
        }

        scrollEvent.handled = true;
    }

private:
    void HandleSceneMouseEvent() {

        if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered())
        {
            isDragging = false;
            return;
        }

        // check if the mouse is hovering over the scene window
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();


        if (mousePos.x >= windowPos.x + contentMin.x && mousePos.x <= windowPos.x + contentMax.x &&
            mousePos.y >= windowPos.y + contentMin.y && mousePos.y <= windowPos.y + contentMax.y)
        {
            // Mouse is inside the content area
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                // Handle left mosue click
                isDragging = true;
            }
            else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            {
                // Handle right mouse release
            }
            if (io.MouseWheel != 0.0f)
            {
                float yoffset = io.MouseWheel;

                float len = glm::length(cam->GetPosition());
                cam->SetPosition(glm::normalize(cam->GetPosition()) * glm::clamp(len + -1.0f * (float)yoffset, 1.0f, 100.0f));
                zoom += yoffset * zoom;
                zoom = std::max(zoom, 0.000001f);
                zoom = std::min(zoom, 3.0f);
            }
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            isDragging = false;
        }

        if (isDragging)
        {
            // click and drag
            float xpos = mousePos.x;
            float ypos = mousePos.y;

            // Calculate the offset of the cursor from the last position
            float xOffset = xpos - lastMousePos.x;
            float yOffset = ypos - lastMousePos.y; // Invert Y-axis if needed

            // Update the position of the dragged object (e.g., camera, object, etc.)
            RotateCamera(xOffset, yOffset);

            centerPos.x -= xOffset * zoom * 2 * renderTargetSize.x / renderTargetSize.y;
            centerPos.y += yOffset * zoom * 2;
        }

        // Update the last cursor position for the next frame
        lastMousePos = { mousePos.x, mousePos.y };
    }

    void HandleSceneWindowResize()
    {
        if (renderTargetSize == imContentSize)
            return;

        renderTargetSize = imContentSize;
        float width = imContentSize.x;
        float height = imContentSize.y;

        // resize textures
        renderTarget->Init(width, height);
        swapTarget->Init(width, height);

        // update shader work groups
        shader->SetWorkGroups(std::ceil(width / 8.0), std::ceil(height / 8.0), 1);
        //mandelbrotShader->SetWorkGroups(std::ceil(width / 8.0), std::ceil(height / 8.0), 1);
        //helloShader->SetWorkGroups(std::ceil(width / 8.0), std::ceil(height / 8.0), 1);
    }
};
