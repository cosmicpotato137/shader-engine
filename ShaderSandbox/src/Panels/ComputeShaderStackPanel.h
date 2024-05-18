#pragma once

#include "GUI/Panel.h"
#include "GUI/ComputeShaderPanel.h"
#include "Core/Serial.h"
#include "Core/FileDialog.h"
#include <map>

class ComputeShaderStackPanel : public Panel {
  std::vector<ComputeShaderPanel> m_ComputeShaderPanels;
  std::vector<int> m_ComputeShaderOrder;
  int m_SelectedIndex = -1;
  int m_DraggingIdx = -1;
  int m_DropTargetIdx = -1;

  // Initialize file dialog with map of extensions
  FileDialog m_FileDialog =
      FileDialog({std::make_pair("Compute Shader", "compute")});

public:
  ComputeShaderStackPanel(const std::string &name = "Compute Shader Stack")
      : Panel(name) {}
  void ImGuiRender() override;
  void Render(const Renderer &ren) override;
  void Update(double dt) override;

  void NewComputeShader(
      const std::string &filepath, std::string name = std::string());
  void AddComputeShader(std::shared_ptr<ComputeShader> shader);
  void DeleteSelectedComputeShader();

private:
  void DragAndDropList();

  SE_SERIAL_FRIENDS;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    // Serialize base class
    ar &boost::serialization::base_object<Panel>(*this);
    ar & m_ComputeShaderPanels;
    ar & m_ComputeShaderOrder;
    ar & m_SelectedIndex;
  }
};