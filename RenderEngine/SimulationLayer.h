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
        computeObject(
            "shader", std::string(SHADER_DIR) + "/compute/blur.compute") {
    // Initialize renderer
    auto app = Application::GetInstance();
    glm::vec2 size = app->GetWindowSize();
    ren.Init(size.x, size.y);

    // Deserialize compute object
    if (std::filesystem::exists("data/simulation.dat"))
      Object::Load(computeObject, "data/simulation.dat");

    // Simulation
    int sep = 3;
    int off = 2;
    for (int i = 0; i < 1000; i += 1) {
      for (int j = 0; j < 100; j += 1) {
        startingAgents.push_back(
            Agent{{off + i * sep, off + j * sep}, {10, 0}});
      }
    }

    sim.SetSize(size);
    sim.Init(startingAgents);

    Renderer::SetClearColor(0, 0, 0);
  }

  ~SimulationLayer() {
    // Serialize compute object
    Object::Save(computeObject, "data/simulation.dat");
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

  virtual void Update(double dt) override { elapsedTime += dt; }

  virtual void Render() override {
    sim.Render(ren);

    // Render simulation layer This also copies renderTarget to swapTarget
    ptr<ComputeShader> shader = computeObject.GetShader();
    BindRenderImages(computeObject.GetShader());
    SetShaderUniforms(shader);
    computeObject.Render();

    // Swaps buffers
    ren.Swap();
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
    ImVec2 s = ImGui::GetContentRegionAvail();
    imContentSize = {s.x, s.y};

    HandleSceneWindowResize();
    HandleSceneMouseEvent();

    // Get the texture ID
    int textureID = ren.GetRenderTarget()->GetTexture()->GetTextureID();

    // Render image to layer
    windowPos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        reinterpret_cast<void *>(static_cast<intptr_t>(textureID)),
        ImVec2(windowPos),
        ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
        ImVec2(0, 1),
        ImVec2(1, 0));

    ImGui::End();  // Scene

    // DEBUG SWAP TARGET WINDOW
    // ImGui::Begin("Scene - debug swap target");
    // WindowPos = ImGui::GetCursorScreenPos();
    // S = ImGui::GetContentRegionAvail();
    // // Render image to layer
    // ImGui::GetWindowDrawList()->AddImage(
    //     (void*)swapTarget->GetTexture()->GetTextureID(),
    //     ImVec2(windowPos),
    //     ImVec2(windowPos.x + s.x, windowPos.y + s.y),
    //     ImVec2(0, 1), ImVec2(1, 0)
    // );
    // ImGui::End();

    ImGui::PopStyleVar(3);
  }

  HANDLE_EVENT_FN(KeyboardEvent) override {
    if (ImGui::GetIO().WantCaptureKeyboard) {
      e.handled = true;
      return;
    }

    int key = e.key;
    int scancode = e.scancode;
    int action = e.action;
    int mods = e.mods;

    if (key == GLFW_KEY_ESCAPE) {
      Application::GetInstance()->SetWindowShouldClose(true);
      e.handled = true;
    }
    if (key == GLFW_KEY_SPACE && action == 1) {
    }
    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action != 0) {
      sim.Step();
    }
    if (key == GLFW_KEY_R && action == 1) {
      sim.Init(startingAgents);
      ren.Clear();
    }
  }

  HANDLE_EVENT_FN(CursorMovedEvent) override {
    if (ImGui::GetIO().WantSetMousePos) {
      e.handled = true;
      return;
    }
  }

  HANDLE_EVENT_FN(MouseButtonEvent) override {
    if (ImGui::GetIO().WantCaptureMouse) {
      e.handled = true;
      return;
    }
  }

  HANDLE_EVENT_FN(ScrollEvent) override {
    if (ImGui::GetIO().WantCaptureMouse) {
      e.handled = true;
      return;
    }

    e.handled = true;
  }

private:
  void HandleSceneMouseEvent() {}

  void HandleSceneWindowResize() {
    if (ren.GetRenderTarget()->GetSize() == imContentSize)
      return;

    float width = imContentSize.x;
    float height = imContentSize.y;

    // Resize textures
    ren.Init(width, height);

    // Update shader work groups
    computeObject.GetShader()->SetWorkGroups(
        std::ceil(width / 8), std::ceil(height / 8), 1);

    // Resize simulation
    sim.SetSize({width, height});
    sim.Init(startingAgents);
  }
};
