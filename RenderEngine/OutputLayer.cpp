#include "OutputLayer.h"
#include "Log.h"

OutputLayer::OutputLayer() : ApplicationLayer("Console") {
  Console::SetCallback([this](const std::string &str) {
    consoleLogs.push_back(str);
    std::cout << str << std::endl; // also push to the console for now
  });
}

OutputLayer::~OutputLayer() {
  // Reset the callback
  Console::SetCallback([](const std::string &str) {
    std::cout << str << std::endl;
  });
}

void OutputLayer::ImGuiRender() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Set window padding to 0
  ImGui::Begin("Output");

  for (size_t i = 0; i < consoleLogs.size(); ++i) {
    if (i % 2 == 0) {
      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 60, 255)); // Dark gray color
    } else {
      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(80, 80, 80, 255)); // Slightly lighter gray color
    }

    // set the margin around the text
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // Set child rounding to create a margin
    ImGui::BeginChild(("line" + std::to_string(i)).c_str(), ImVec2(-1, ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(cursor + ImVec2(10, 0));
    ImGui::Text(consoleLogs[i].c_str());
    ImGui::EndChild();
    ImGui::PopStyleVar(); // Don't forget to pop the style var after using it

    ImGui::PopStyleColor(); // Don't forget to pop the style color after using it
  }

  ImGui::End();
  ImGui::PopStyleVar(); // Don't forget to pop the style var after using it
}


void OutputWindow() {

}