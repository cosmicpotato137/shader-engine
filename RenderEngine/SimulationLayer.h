#pragma once

#include "Application.h"
#include "ApplicationLayer.h"
#include "ComputeObject.h"
#include "ComputeShader.h"
#include "Log.h"
#include "Mesh.h"
#include "RenderTexture.h"
#include "Renderer.h"
#include "Shader.h"
#include "Simulation.h"
#include "Texture.h"

#include "re_core.h"

class SimulationLayer : public ApplicationLayer {

  // Render vars
  Renderer ren;

  ComputeObject computeObject;

  // Event vars
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

  // Imgui vars
  glm::vec2 imContentSize;

  // Simulation!
  Simulation sim;
  std::vector<Agent> startingAgents;

public:
  SimulationLayer()
      : ApplicationLayer("main render layer"),
        computeObject("shader", SHADER_DIR + "/compute/blur.compute") {
    // Initialize renderer
    auto app = Application::GetInstance();
    glm::vec2 size = app->GetWindowSize();
    ren.Init(size.x, size.y);

    // Simulation
    int sep = 1;
    int off = 10;
    for (int i = 0; i < 1000; i += 1) {
      for (int j = 0; j < 400; j += 1) {
        startingAgents.push_back(
            Agent{{off + i * sep, off + j * sep}, {10, 0}});
      }
    }

    sim.Init(startingAgents);

    Renderer::SetClearColor(0, 0, 0);
  }

  void BindRenderImages(ptr<Shader> shader) {
    ren.GetRenderTarget()->GetTexture()->BindCompute(0);
    shader->SetUniform("imageOut", 0);
    // Bind swap target for iterative sims
    if (shader->HasUniform("imageIn")) {
      shader->SetUniform("imageIn", 1);
      ren.GetSwapTarget()->GetTexture()->BindCompute(1);
    }
  }

  void SetShaderUniforms(ptr<Shader> shader) {
    // Set default uniforms
    if (shader->HasUniform("_time"))
      shader->SetUniform("_time", elapsedTime);
  }

  virtual void Update(double dt) override {
    if (!pause || step) {
      elapsedTime += dt;
      sim.Update(.1f);
    }
  }

  virtual void Render() override {
    if (pause && !step)
      return;

    sim.Render(ren);

    // Render simulation layer
    // This also copies renderTarget to swapTarget
    ptr<ComputeShader> shader = computeObject.GetShader();
    BindRenderImages(computeObject.GetShader());
    SetShaderUniforms(shader);
    computeObject.Render();

    // Swaps buffers
    ren.Render();

    step = false;
  }

  virtual void ImGuiRender() override {
    SceneWindow();
    computeObject.ImGuiRender();
    sim.ImGuiRender();
  }

  void SceneWindow() {
    // Scene window styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {3.0, 3.0});

    ImGui::Begin("Scene");

    HandleSceneWindowResize();
    HandleSceneMouseEvent();

    ImVec2 s = ImGui::GetContentRegionAvail();
    imContentSize = {s.x, s.y};

    // Render image to layer
    windowPos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void *)ren.GetRenderTarget()->GetTexture()->GetTextureID(),
        ImVec2(windowPos),
        ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
        ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    // DEBUG SWAP TARGET WINDOW
    // ImGui::Begin("Scene - debug swap target");
    //
    // WindowPos = ImGui::GetCursorScreenPos();
    //// Render image to layer
    // ImGui::GetWindowDrawList()->AddImage(
    //    (void*)swapTarget->GetTexture()->GetTextureID(),
    //    ImVec2(windowPos),
    //    ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
    //    ImVec2(0, 1), ImVec2(1, 0)
    //);
    // ImGui::End();

    ImGui::PopStyleVar(3);
  }

  virtual void HandleEvent(KeyboardEvent &keyEvent) override {
    if (ImGui::GetIO().WantCaptureKeyboard) {
      keyEvent.handled = true;
      return;
    }

    int key = keyEvent.key;
    int scancode = keyEvent.scancode;
    int action = keyEvent.action;
    int mods = keyEvent.mods;

    if (key == GLFW_KEY_ESCAPE) {
      Application::GetInstance()->SetWindowShouldClose(true);
      keyEvent.handled = true;
    }
    if (key == GLFW_KEY_SPACE && action == 1) {
      pause = !pause;
    }
    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action != 0) {
      step = true;
      sim.Step();
    }
    if (key == GLFW_KEY_R && action == 1) {
      sim.Init(startingAgents);
    }
  }

  virtual void HandleEvent(CursorMovedEvent &cursorMoved) override {
    if (ImGui::GetIO().WantSetMousePos) {
      cursorMoved.handled = true;
      return;
    }
  }

  virtual void HandleEvent(MouseButtonEvent &mouseButton) override {
    if (ImGui::GetIO().WantCaptureMouse) {
      mouseButton.handled = true;
      return;
    }
  }

  virtual void HandleEvent(ScrollEvent &scrollEvent) override {
    if (ImGui::GetIO().WantCaptureMouse) {
      scrollEvent.handled = true;
      return;
    }

    scrollEvent.handled = true;
  }

private:
  void HandleSceneMouseEvent() {

    // Check if the mouse is hovering over the scene window
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
    ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

    if (!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered()) {
      lmbDown = false;
      rmbDown = false;
    } else if (mousePos.x >= windowPos.x + contentMin.x &&
               mousePos.x <= windowPos.x + contentMax.x &&
               mousePos.y >= windowPos.y + contentMin.y &&
               mousePos.y <= windowPos.y + contentMax.y) {
      // Mouse is inside the content area
      if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        // Handle left mosue click
        lmbDown = true;
      }
      if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        // Handle right mouse release
        rmbDown = true;
      }
      float yoffset = io.MouseWheel;
      if (io.MouseWheel != 0.0f) {
        zoom += io.MouseWheel;
      }
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
      lmbDown = false;
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
      rmbDown = false;

    if (lmbDown) {
      // Click and drag
      float xpos = mousePos.x;
      float ypos = mousePos.y;

      // Calculate the offset of the cursor from the last position
      float xOffset = xpos - lastMousePos.x;
      float yOffset = ypos - lastMousePos.y;  // Invert Y-axis if needed

      centerPos.x -= xOffset * zoom * 2 * ren.GetAspect();
      centerPos.y += yOffset * zoom * 2;
    }

    // Update the last cursor position for the next frame
    lastMousePos = {mousePos.x, mousePos.y};
  }

  void HandleSceneWindowResize() {
    if (ren.GetSize() == imContentSize)
      return;

    // Set width
    float width = imContentSize.x;
    float height = imContentSize.y;
    ren.Init(width, height);
    ren.Clear();

    // Resize textures
    /*renderTarget->Init(width, height, false);
    renderTarget->Clear();
    swapTarget->Init(width, height, false);
    swapTarget->Clear();*/

    // Update shader work groups
    computeObject.GetShader()->SetWorkGroups(std::ceil(width / 8),
                                             std::ceil(height / 8), 1);

    // Resize simulation
    sim.SetSize({width, height});
    sim.Init(startingAgents);
  }
};
