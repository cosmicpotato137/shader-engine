#pragma once

#include "core.h"

#include "ApplicationLayer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

class Application;

class ImGuiLayer : public ApplicationLayer {
public:
  ImGuiLayer();

  ~ImGuiLayer() {}

  // Begin application dockspace
  void ImGuiBegin();

  // End application dockspace
  void ImGuiEnd();

  // Render dockspace
  void ImGuiRender() override;

  void Render() override {}

  void Update(double dt) override {}

  // Event handling
  HANDLE_EVENT_FN(MouseButtonEvent) override;

  HANDLE_EVENT_FN(WindowResizeEvent) override {}

  HANDLE_EVENT_FN(KeyboardEvent) override {}

protected:
  // Called when layer is added to the application
  bool Init() override;

  // Called when layer is removed from the application
  void Cleanup() override;
};