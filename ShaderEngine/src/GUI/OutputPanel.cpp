#include "OutputPanel.h"
#include "Core/Log.h"
#include "imgui.h"

OutputPanel::OutputPanel() : Panel("Output") {
  Console::SetCallback([this](const std::string &str) {
    m_LastLogCount = m_ConsoleLogs.size();
    if (m_ConsoleLogs.empty() || m_ConsoleLogs.back().first != str) {
      m_ConsoleLogs.push_back({str, 1});
    } else {
      m_ConsoleLogs.back().second++;
    }
    std::cout << str << std::endl;  // also push to the console for now
  });
}

void OutputPanel::ImGuiRender() {
  // no window padding
  ImGui::PushStyleVar(
      ImGuiStyleVar_WindowPadding, ImVec2(0, 0));  // Set window padding to 0
  ImGui::PushStyleVar(
      ImGuiStyleVar_ItemSpacing, ImVec2(10, 5));  // Set item spacing to 0

  ImGui::Begin(m_Name.c_str());

  // Get window size
  ImVec2 m_Size = ImGui::GetWindowSize();

  // Add top padding
  ImVec2 cursor = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursor + ImVec2(0, 5));

  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, 90);

  // clear logs
  if (ImGui::Button("Clear")) {
    m_ConsoleLogs.clear();
  }

  ImGui::NextColumn();

  // auto scroll
  ImGui::Checkbox("Auto-scroll", &m_AutoScroll);

  ImGui::Columns(1);

  // Calculate the remaining space
  float child_height = ImGui::GetContentRegionAvail().y;

  // Scrollable content
  ImGui::BeginChild("ScrollingRegion", ImVec2(0, child_height), false);

  // align boxes correctly
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));

  size_t i = 0;
  for (auto &log : m_ConsoleLogs) {
    if (i % 2 == 0) {
      ImGui::PushStyleColor(
          ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 255));  // Dark gray color
    } else {
      ImGui::PushStyleColor(
          ImGuiCol_ChildBg,
          IM_COL32(50, 50, 50, 255));  // Slightly lighter gray color
    }

    int ypad = 5;
    int xpad = 10;

    std::string text = log.first;

    // Log the number of identical logs
    if (log.second > 1) {
      text += " (" + std::to_string(log.second) + ")";
    }
    ImVec2 textSize = ImGui::CalcTextSize(
        text.c_str(), nullptr, true, ImGui::GetWindowWidth() - xpad * 2);

    ImGui::BeginChild(
        ("line" + std::to_string(i)).c_str(),
        ImVec2(-1, textSize.y + 2 * ypad),
        ImGuiChildFlags_AlwaysUseWindowPadding);

    // add top padding
    cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(cursor + ImVec2(xpad, ypad));
    ImGui::PushTextWrapPos(
        ImGui::GetWindowWidth() - xpad);  // Wrap text at the edge of the window
    ImGui::TextUnformatted(text.c_str());  // Display the text
    ImGui::PopTextWrapPos();               // Stop text wrapping
    // add bottom padding
    cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(cursor + ImVec2(0, ypad));

    ImGui::EndChild();
    ImGui::PopStyleColor();

    i++;
  }

  // Scroll to the end of the content
  if (m_AutoScroll && m_LastLogCount < m_ConsoleLogs.size()) {
    ImGui::SetScrollHereY(1.0f);
  }

  ImGui::PopStyleVar();
  ImGui::EndChild();

  ImGui::End();           // End the output window
  ImGui::PopStyleVar(2);  // Don't forget to pop the style var after using it

  m_LastLogCount = m_ConsoleLogs.size();
}