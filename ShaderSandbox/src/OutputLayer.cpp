#include "OutputLayer.h"
#include "Core/Log.h"

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

void OutputLayer::ImGuiRender() {}

void OutputWindow() {}