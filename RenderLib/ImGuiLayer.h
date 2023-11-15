#pragma once

#include "core.h"

#include "ApplicationLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

class Application;

class ImGuiLayer : public ApplicationLayer {
    Application* app;

public:
    ImGuiLayer(Application* app);

    ~ImGuiLayer() {}

    void ImGuiBegin();

    void ImGuiEnd();

    void ImGuiRender() override;

    void HandleEvent(MouseButtonEvent& e) override;

    void HandleEvent(WindowResizeEvent& e) override {}

    void HandleEvent(KeyboardEvent& e) override {}
};