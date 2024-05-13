#pragma once

#include "Core/ApplicationLayer.h"
#include "imgui.h"

#include "Renderer/Renderer.h"
#include "Renderer/ComputeShader.h"
#include "GUI/OutputPanel.h"
#include "GUI/ScenePanel.h"
#include "GUI/ComputeShaderPanel.h"
#include "Panels/ComputeShaderStackPanel.h"

class EditorLayer : public ApplicationLayer {
  OutputPanel m_OutputPanel;
  ScenePanel m_ScenePanel;
  ComputeShaderStackPanel m_ComputeShaderStackPanel;
  ptr<Renderer> m_Renderer;
  ptr<ComputeShader> m_ComputeShader;

  std::filesystem::path m_DataPath =
      std::filesystem::current_path() / "bin/Debug/data";

public:
  EditorLayer();

  ~EditorLayer();

  virtual bool OnAttach() override;
  virtual void OnDetach() override;

  // Render dockspace
  void ImGuiRender() override;

  void Render() override;

  void Update(double dt) override;

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