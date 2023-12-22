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
#include "ComputeObject.h"
#include "Simulation.h"

#include "re_core.h"

class SimulationLayer : public ApplicationLayer {

    // render vars
    Renderer ren;
    ptr<RenderTexture> renderTarget;
    ptr<RenderTexture> swapTarget;

    ptr<Mesh> quad;
    ptr<Shader> colorShader;

    ComputeObject computeObject;

    // event vars
    float elapsedTime = 0.0f;
    bool pause = false;
    bool reset = false;
    bool step = false;

    ImVec2 windowPos = ImVec2(0, 0);
    glm::vec2 lastMousePos = glm::vec2(0, 0);
    bool lmbDown = false;
    bool rmbDown = false;

    glm::vec2 centerPos = glm::vec2(0);
    float zoom = 1.0f;

    // imgui vars
    glm::vec2 imContentSize;
    glm::vec2 renderTargetSize;

    // simulation!
    Simulation sim;
    std::vector<Agent> startingAgents;

public:
    SimulationLayer()
        : ApplicationLayer("main render layer"),
        computeObject("shader", SHADER_DIR + "/compute/simulation/slime.compute")
    {
        glm::vec2 screenSize = renderTargetSize;
        int screenWidth = screenSize.x;
        int screenHeight = screenSize.y;

        // assign the render target texture to the compute shader's buffer
        renderTarget = std::make_shared<RenderTexture>();
        renderTarget->Init(screenWidth, screenHeight, false);

        // make render target to swap into
        swapTarget = std::make_shared<RenderTexture>();
        swapTarget->Init(screenWidth, screenHeight, false);

        // initialize mesh
        quad = Mesh::Quad();

        // initialize shader
        colorShader = std::make_shared<Shader>("color2D");
        colorShader->Init(SHADER_DIR + "/color2D.shader");
        colorShader->SetUniform("color", glm::vec3(1, 0, 0));

        // simulation
        for (int i = 0; i < 20; i += 1)
        {
            for (int j = 0; j < 20; j += 1)
            {
                startingAgents.push_back(Agent{ { 10 + i * 10, 10 + j * 10}, { 10, 5 } });
            }
        }

        sim.Init(startingAgents);

        Renderer::SetClearColor(0, 0, 0);
    }

    void BindRenderImages(ptr<Shader> shader)
    {
        renderTarget->GetTexture()->BindCompute(0);
        shader->SetUniform("imageOut", 0);
        // bind swap target for iterative sims
        if (shader->HasUniform("imageIn"))
        {
            shader->SetUniform("imageIn", 1);
            swapTarget->GetTexture()->BindCompute(1);
        }
    }

    void SetShaderUniforms(ptr<Shader> shader)
    {
        // set default uniforms
        if (shader->HasUniform("_time"))
            shader->SetUniform("_time", elapsedTime);
        //if (shader->HasUniform("_mouse_position"))
        //{
        //    glm::vec2 pos = glm::vec2(lastMousePos.x - windowPos.x, renderTargetSize.y - (lastMousePos.y - windowPos.y));
        //    shader->SetUniform("_mouse_position", pos);
        //}
        //if (shader->HasUniform("_lmb_down"))
        //    shader->SetUniform("_lmb_down", lmbDown);
        //if (shader->HasUniform("_rmb_down"))
        //    shader->SetUniform("_rmb_down", rmbDown);
    }

    virtual void Update(double dt) override {
        if (!pause || step)
        {
            elapsedTime += dt;
            sim.Update(.1f);
        }
    }

    virtual void Render() override {
        if (pause && !step)
            return;

        sim.Render(swapTarget);

        // render simulation layer
        // this also copies renderTarget to swapTarget
        ptr<ComputeShader> shader = computeObject.GetShader();
        BindRenderImages(computeObject.GetShader());
        SetShaderUniforms(shader);
        computeObject.Render();

        // swap
        renderTarget.swap(swapTarget);

        step = false;
    }

    virtual void ImGuiRender() override
    {
        SceneWindow();
        computeObject.ImGuiRender();
        sim.ImGuiRender();
    }

    void SceneWindow()
    {
        // scene window styling
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 3.0, 3.0 });

        ImGui::Begin("Scene");

        HandleSceneWindowResize();
        HandleSceneMouseEvent();

        ImVec2 s = ImGui::GetContentRegionAvail();
        imContentSize = { s.x, s.y };

        // render image to layer
        windowPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            (void*)renderTarget->GetTexture()->GetTextureID(),
            ImVec2(windowPos),
            ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
            ImVec2(0, 1), ImVec2(1, 0)
        );

        ImGui::End();

        // DEBUG SWAP TARGET WINDOW
        //ImGui::Begin("Scene - debug swap target");
        //
        //windowPos = ImGui::GetCursorScreenPos();
        //// render image to layer
        //ImGui::GetWindowDrawList()->AddImage(
        //    (void*)swapTarget->GetTexture()->GetTextureID(),
        //    ImVec2(windowPos),
        //    ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
        //    ImVec2(0, 1), ImVec2(1, 0)
        //);
        //ImGui::End();

        ImGui::PopStyleVar(3);
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
            Application::GetInstance()->SetWindowShouldClose(true);
            keyEvent.handled = true;
        }
        if (key == GLFW_KEY_SPACE && action == 1)
        {
            pause = !pause;
        }
        if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action != 0)
        {
            step = true;
            sim.Step();
        }
        if (key == GLFW_KEY_R && action == 1)
        {
            sim.Init(startingAgents);
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

        // check if the mouse is hovering over the scene window
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();


        if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered())
        {
            lmbDown = false;
            rmbDown = false;
        }
        else if (mousePos.x >= windowPos.x + contentMin.x && mousePos.x <= windowPos.x + contentMax.x && mousePos.y >= windowPos.y + contentMin.y && mousePos.y <= windowPos.y + contentMax.y)
        {
            // Mouse is inside the content area
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                // Handle left mosue click
                lmbDown = true;
            }
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                // Handle right mouse release
                rmbDown = true;
            }
            float yoffset = io.MouseWheel;
            if (io.MouseWheel != 0.0f)
            {
                zoom += io.MouseWheel;
            }
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            lmbDown = false;
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            rmbDown = false;

        if (lmbDown)
        {
            // click and drag
            float xpos = mousePos.x;
            float ypos = mousePos.y;

            // Calculate the offset of the cursor from the last position
            float xOffset = xpos - lastMousePos.x;
            float yOffset = ypos - lastMousePos.y; // Invert Y-axis if needed

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
        renderTarget->Init(width, height, false);
        renderTarget->Clear();
        swapTarget->Init(width, height, false);
        swapTarget->Clear();


        Renderer::SetViewport(0, 0, width, height);

        Console::Log("window size %f %f", Application::GetInstance()->GetWindowSize().x, Application::GetInstance()->GetWindowSize().y);
        Console::Log("screen size %f %f", width, height);
        // Update shader work groups
        computeObject.GetShader()->SetWorkGroups(std::ceil(width / 8), std::ceil(height / 8), 1);

        // resize simulation
        sim.SetSize({ width, height });
        sim.Init(startingAgents);
    }
};
