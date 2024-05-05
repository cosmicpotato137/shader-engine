#pragma once

#include "Core/ApplicationLayer.h"
#include "imgui.h"

#include "Renderer/Renderer.h"
#include "Renderer/ComputeShader.h"
#include "ImGui/OutputPanel.h"
#include "ImGui/ScenePanel.h"

class EditorLayer : public ApplicationLayer {
  OutputPanel m_OutputPanel;
  ScenePanel m_ScenePanel;
  ptr<Renderer> m_Renderer;
  ptr<ComputeShader> m_ComputeShader;

public:
  EditorLayer();

  ~EditorLayer();

  virtual bool OnAttach() override;
  virtual void OnDetach() override;

  // Render dockspace
  void ImGuiRender() override;

  void Render() override;

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