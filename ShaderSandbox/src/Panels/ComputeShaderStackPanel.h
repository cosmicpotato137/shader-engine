#pragma once

#include "GUI/Panel.h"
#include "GUI/ComputeShaderPanel.h"
#include "Core/Serial.h"
#include "Core/FileDialog.h"
#include <map>

class ComputeShaderStackPanel : public Panel {
  std::vector<ptr<ComputeShaderPanel>> m_ComputeShaderPanels;
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

  template <class T>
  void NewComputeShaderPanel(
      const std::string &filepath, std::string name = std::string());
  void DeleteSelectedComputeShader();

private:
  void DragAndDropList();

  SE_SERIAL_FRIENDS;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    // Serialize base classt
    ar &boost::serialization::base_object<Panel>(*this);
    ar & m_ComputeShaderPanels;
    ar & m_SelectedIndex;
  }
};

template <class T>
void ComputeShaderStackPanel::NewComputeShaderPanel(
    const std::string &filepath, std::string name) {
  if (name == "") {
    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    name = filename.substr(0, filename.find_last_of('.'));
  }
  auto shader = std::make_shared<ComputeShader>(name);

  m_ComputeShaderPanels.emplace_back(std::make_shared<T>(shader));

  m_SelectedIndex = m_ComputeShaderPanels.size() - 1;
  if (!shader->Init(filepath)) {
    Console::Log("Failed to load compute shader: %s", name);
    DeleteSelectedComputeShader();
    m_SelectedIndex = -1;
  }
}
