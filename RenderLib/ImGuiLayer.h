#pragma once

#include "core.h"

#include "ApplicationLayer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

class Application;

class ImGuiLayer : public ApplicationLayer {
  Application *app;

public:
  ImGuiLayer(Application *app);

  ~ImGuiLayer() {}

  // Begin application dockspace
  void ImGuiBegin();

  // End application dockspace
  void ImGuiEnd();

  void ImGuiRender() override;

  void HandleEvent(MouseButtonEvent &e) override;

  void HandleEvent(WindowResizeEvent &e) override {}

  void HandleEvent(KeyboardEvent &e) override {}
};