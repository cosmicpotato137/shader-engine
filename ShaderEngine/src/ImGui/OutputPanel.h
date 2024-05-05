#pragma once

#include "ImGui/Panel.h"
#include <vector>

class OutputPanel : public Panel {
  int m_LastLogCount = 0;
  std::vector<std::string> m_ConsoleLogs;
  bool m_AutoScroll = true;

public:
  OutputPanel();
  virtual ~OutputPanel() = default;

  virtual void ImGuiRender() override;
};