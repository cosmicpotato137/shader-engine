#pragma once

#include "Core/ApplicationLayer.h"

#include "imgui.h"

class OutputLayer : public ApplicationLayer {
  std::vector<std::string> consoleLogs;

  bool autoScroll = true;
  int lastLogCount = 0;

public:
  OutputLayer();

  ~OutputLayer();

  // Render dockspace
  void ImGuiRender() override;

  void Render() override {}

  void Update(double dt) override {}

  // Event handling
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
};