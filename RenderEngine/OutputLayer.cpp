#include "OutputLayer.h"
#include "Log.h"

OutputLayer::OutputLayer() : ApplicationLayer("Console") {
  Console::SetCallback([this](const std::string &str) {
    lastLogCount = consoleLogs.size();
    consoleLogs.push_back(str);
    std::cout << str << std::endl;  // also push to the console for now
  });
}

OutputLayer::~OutputLayer() {
  // Reset the callback
  Console::SetCallback(
      [](const std::string &str) { std::cout << str << std::endl; });
}

void OutputLayer::ImGuiRender() {
  // no window padding
  ImGui::PushStyleVar(
      ImGuiStyleVar_WindowPadding, ImVec2(0, 0));  // Set window padding to 0
  ImGui::PushStyleVar(
      ImGuiStyleVar_ItemSpacing, ImVec2(10, 5));  // Set item spacing to 0

  ImGui::Begin("Output");

  // the cursor position
  ImVec2 cursor;

  // add top padding
  cursor = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursor + ImVec2(0, 5));

  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, 90);

  // clear logs
  if (ImGui::Button("Clear")) {
    consoleLogs.clear();
  }

  ImGui::NextColumn();

  // auto scroll
  ImGui::Checkbox("Auto-scroll", &autoScroll);

  ImGui::Columns(1);

  // Calculate the remaining space
  float child_height = ImGui::GetContentRegionAvail().y;

  // Scrollable content
  ImGui::BeginChild("ScrollingRegion", ImVec2(0, child_height), false);

  // align boxes correctly
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));

  for (size_t i = 0; i < consoleLogs.size(); ++i) {
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

    std::string text = consoleLogs[i];
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
    ImGui::TextUnformatted(consoleLogs[i].c_str());  // Display the text
    ImGui::PopTextWrapPos();                         // Stop text wrapping
    // add bottom padding
    cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(cursor + ImVec2(0, ypad));

    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  // Scroll to the end of the content
  if (autoScroll && lastLogCount < consoleLogs.size()) {
    ImGui::SetScrollHereY(1.0f);
  }

  ImGui::PopStyleVar();
  ImGui::EndChild();

  ImGui::End();           // End the output window
  ImGui::PopStyleVar(2);  // Don't forget to pop the style var after using it

  lastLogCount = consoleLogs.size();
}

void OutputWindow() {}