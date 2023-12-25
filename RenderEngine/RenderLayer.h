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
#include "Texture.h"

#include "re_core.h"

class RenderLayer : public ApplicationLayer {

  // Render vars
  Renderer ren;
  ptr<RenderTexture> renderTarget;
  ptr<RenderTexture> swapTarget;
  ptr<Camera> cam;

  ComputeObject computeInteract;
  ComputeObject computeObject;

  // Event vars
  float elapsedTime = 0.0f;
  bool pause = false;
  bool reset = false;

  ImVec2 windowPos = ImVec2(0, 0);
  glm::vec2 lastMousePos = glm::vec2(0, 0);
  bool lmbDown = false;
  bool rmbDown = false;

  glm::vec2 centerPos = glm::vec2(0);
  float zoom = 1.0f;

  // Imgui vars
  glm::vec2 imContentSize;
  glm::vec2 renderTargetSize;

public:
  RenderLayer()
      : ApplicationLayer("main render layer"),
        computeObject("shader", SHADER_DIR + "/compute/conway.compute"),
        computeInteract("interact",
                        SHADER_DIR + "/compute/conwayInteract.compute") {
    // Renderer settings
    cam = ren.GetMainCamera();

    ren.SetClearColor(.3, .3, .3);
    ren.Clear();

    glm::vec2 screenSize = renderTargetSize;
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    // Assign the render target texture to the compute shader's buffer
    renderTarget = std::make_shared<RenderTexture>();
    renderTarget->Init(screenWidth, screenHeight, true);
    ren.SetRenderTarget(renderTarget);

    // Make render target to swap into
    swapTarget = std::make_shared<RenderTexture>();
    swapTarget->Init(screenWidth, screenHeight, true);
  }

  void BindRenderImages(ptr<Shader> shader) {
    renderTarget->GetTexture()->BindCompute(0);
    shader->SetUniform("imageOut", 0);
    // Bind swap target for iterative sims
    if (shader->HasUniform("imageIn")) {
      shader->SetUniform("imageIn", 1);
      swapTarget->GetTexture()->BindCompute(1);
    }
  }

  void SetShaderUniforms(ptr<Shader> shader) {
    // Set default uniforms
    if (shader->HasUniform("_center"))
      shader->SetUniform("_center", centerPos / renderTargetSize);
    if (shader->HasUniform("_time"))
      shader->SetUniform("_time", elapsedTime);
    if (shader->HasUniform("_scale"))
      shader->SetUniform("_scale", zoom);
    if (shader->HasUniform("_max_iterations"))
      shader->SetUniform("_max_iterations",
                         unsigned int(30 / std::pow(zoom, 1.0 / 3.0)));
    if (shader->HasUniform("_mouse_position")) {
      glm::vec2 pos =
          glm::vec2(lastMousePos.x - windowPos.x,
                    renderTargetSize.y - (lastMousePos.y - windowPos.y));
      shader->SetUniform("_mouse_position", pos);
    }
    if (shader->HasUniform("_lmb_down"))
      shader->SetUniform("_lmb_down", lmbDown);
    if (shader->HasUniform("_rmb_down"))
      shader->SetUniform("_rmb_down", rmbDown);
  }

  virtual void Update(double dt) override {
    if (!pause)
      elapsedTime += dt;
  }

  virtual void Render() override {
    // Render interaction layer
    ptr<ComputeShader> interactShader = computeInteract.GetShader();
    SetShaderUniforms(interactShader);
    computeInteract.Render();

    // Render simulation layer
    ptr<ComputeShader> shader = computeObject.GetShader();
    BindRenderImages(shader);
    SetShaderUniforms(shader);
    computeObject.Render();

    // Swap
    renderTarget.swap(swapTarget);
  }

  virtual void ImGuiRender() override {
    SceneWindow();
    computeObject.ImGuiRender();
    computeInteract.ImGuiRender();
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
        (void *)renderTarget->GetTexture()->GetTextureID(), ImVec2(windowPos),
        ImVec2(windowPos.x + imContentSize.x, windowPos.y + imContentSize.y),
        ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

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

  void RotateCamera(float xOffset, float yOffset) {
    glm::vec3 pos = cam->GetPosition();
    float len = glm::length(pos);

    // Rind the new position for the camera
    float rotspd = .05f * len;
    glm::vec3 rightOffset = cam->GetRight() * -xOffset * rotspd;
    glm::vec3 upOffset = cam->GetUp() * yOffset * rotspd;
    glm::vec3 newPos = glm::normalize(pos + upOffset + rightOffset) * len;

    // Ensure there isn't flipping at the poles
    float d = glm::dot(glm::normalize(newPos), glm::vec3(0, 1, 0));
    if (1 - abs(d) > 0.01f) {
      cam->SetPosition(newPos);
      cam->LookAt({0, 0, 0});
    }
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
    if (key == GLFW_KEY_R) {
      cam->SetPosition({0, 0, -5});
      cam->LookAt({0, 0, 0});
      keyEvent.handled = true;
    }
    if (key == GLFW_KEY_SPACE && action == 1) {
      pause = !pause;
      computeObject.ToggleStop();
    }
    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action != 0) {
      computeObject.Step();
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
        rmbDown = true;
        // Handle right mouse release
      }
      float yoffset = io.MouseWheel;
      if (io.MouseWheel != 0.0f) {
        float len = glm::length(cam->GetPosition());
        cam->SetPosition(
            glm::normalize(cam->GetPosition()) *
            glm::clamp(len + -1.0f * (float)yoffset, 1.0f, 100.0f));
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

      // Update the position of the dragged object (e.g., camera, object, etc.)
      RotateCamera(xOffset, yOffset);

      centerPos.x -=
          xOffset * zoom * 2 * renderTargetSize.x / renderTargetSize.y;
      centerPos.y += yOffset * zoom * 2;
    }

    // Update the last cursor position for the next frame
    lastMousePos = {mousePos.x, mousePos.y};
  }

  void HandleSceneWindowResize() {
    if (renderTargetSize == imContentSize)
      return;

    renderTargetSize = imContentSize;
    float width = imContentSize.x;
    float height = imContentSize.y;

    // Resize textures
    renderTarget->Init(width, height, true);
    swapTarget->Init(width, height, true);

    // Update shader work groups
    computeObject.GetShader()->SetWorkGroups(std::ceil(width / 8),
                                             std::ceil(height / 8), 1);
    computeInteract.GetShader()->SetWorkGroups(std::ceil(width / 8),
                                               std::ceil(height / 8), 1);
    // mandelbrotShader->SetWorkGroups(std::ceil(width / 8.0), std::ceil(height
    // / 8.0), 1); helloShader->SetWorkGroups(std::ceil(width / 8.0),
    // std::ceil(height / 8.0), 1);
  }
};
