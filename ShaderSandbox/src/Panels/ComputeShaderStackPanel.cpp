#include "ComputeShaderStackPanel.h"

void ComputeShaderStackPanel::ImGuiRender() {
  ImGui::Begin(m_Name.c_str());
  if (ImGui::Button("Add Compute Shader")) {
    std::string filepath = m_FileDialog.Show();
    if (filepath != "") {
      NewComputeShader(filepath);
    }
  }

  // Shader stack
  ImGui::Spacing();
  DragAndDropList();

  // Check if the delete key was pressed
  if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
    DeleteSelectedComputeShader();
  }

  ImGui::End();

  // Render inspector
  if (m_SelectedIndex >= 0 && m_SelectedIndex < m_ComputeShaderPanels.size())
    m_ComputeShaderPanels[m_SelectedIndex].ImGuiRender();
}

void ComputeShaderStackPanel::Render(const Renderer &ren) {
  for (ComputeShaderPanel p : m_ComputeShaderPanels) {
    p.Render(ren);
  }
}

void ComputeShaderStackPanel::Update(double dt) {
  for (ComputeShaderPanel p : m_ComputeShaderPanels) {
    p.Update(dt);
  }
}

void ComputeShaderStackPanel::NewComputeShader(
    const std::string &filepath, std::string name) {
  if (name == "") {
    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    name = filename.substr(0, filename.find_last_of('.'));
  }
  auto shader = std::make_shared<ComputeShader>(name);
  AddComputeShader(shader);
  m_SelectedIndex = m_ComputeShaderPanels.size() - 1;
  if (!shader->Init(filepath)) {
    Console::Log("Failed to load compute shader: %s", name);
    DeleteSelectedComputeShader();
    m_SelectedIndex = -1;
  }
}

void ComputeShaderStackPanel::AddComputeShader(
    std::shared_ptr<ComputeShader> shader) {
  ComputeShaderPanel panel(shader);
  m_ComputeShaderPanels.emplace_back(panel);
  m_ComputeShaderOrder.push_back(m_ComputeShaderPanels.size() - 1);
}

void ComputeShaderStackPanel::DeleteSelectedComputeShader() {
  if (m_SelectedIndex >= 0 && m_SelectedIndex < m_ComputeShaderPanels.size()) {
    m_ComputeShaderPanels.erase(
        m_ComputeShaderPanels.begin() + m_SelectedIndex);
    m_ComputeShaderOrder.erase(
        std::remove(
            m_ComputeShaderOrder.begin(),
            m_ComputeShaderOrder.end(),
            m_SelectedIndex),
        m_ComputeShaderOrder.end());

    // Decrement all elements greater than the deleted index
    for (int &index : m_ComputeShaderOrder) {
      if (index > m_SelectedIndex) {
        index--;
      }
    }

    m_SelectedIndex = -1;
  }
}

void ComputeShaderStackPanel::DragAndDropList() {
  for (int n = 0; n < m_ComputeShaderPanels.size(); n++) {
    ImGui::PushID(n);

    std::string name = m_ComputeShaderPanels[n].GetName();
    bool selected = m_SelectedIndex == n;
    if (ImGui::Selectable(name.c_str(), selected)) {
      m_SelectedIndex = n;
    }

    // Our buttons are both drag sources and drag targets here!
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
      // Set payload to carry the index of our item (could be anything)
      ImGui::SetDragDropPayload("COMPUTE_SHADER_STACK", &n, sizeof(int));

      // Display text following mouse
      ImGui::Text("Move %s", name.c_str());

      ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload *payload =
              ImGui::AcceptDragDropPayload("COMPUTE_SHADER_STACK")) {
        Console::Assert(
            payload->DataSize == sizeof(int), "Invalid payload size");
        int payload_n = *(const int *)payload->Data;
        // Insert before
        m_ComputeShaderPanels.insert(
            m_ComputeShaderPanels.begin() + n,
            m_ComputeShaderPanels[payload_n]);
        // Remove the element from its old position
        // Note: the position is payload_n + 1 because the list size has
        // increased by 1
        m_ComputeShaderPanels.erase(
            m_ComputeShaderPanels.begin() + payload_n +
            (payload_n >= n ? 1 : 0));
      }
      ImGui::EndDragDropTarget();
    }
    ImGui::PopID();
  }

  ImGui::PushID(m_ComputeShaderPanels.size());
  // Get window size
  ImVec2 windowSize = ImGui::GetWindowSize();
  // Get standard text height
  float textHeight = ImGui::GetTextLineHeight();
  ImGui::Dummy(ImVec2(windowSize.x, textHeight));

  // Final drop target to place objects at the end of the list
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload *payload =
            ImGui::AcceptDragDropPayload("COMPUTE_SHADER_STACK")) {
      Console::Assert(payload->DataSize == sizeof(int), "Invalid payload size");
      int payload_n = *(const int *)payload->Data;
      m_ComputeShaderPanels.push_back(m_ComputeShaderPanels[payload_n]);
      m_ComputeShaderPanels.erase(m_ComputeShaderPanels.begin() + payload_n);
    }
    ImGui::EndDragDropTarget();
  }
  ImGui::PopID();
}
