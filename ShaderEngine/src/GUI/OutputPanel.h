#pragma once

#include "GUI/Panel.h"
#include <vector>

class OutputPanel : public Panel {
  int m_LastLogCount = 0;
  std::vector<std::pair<std::string, int>> m_ConsoleLogs;
  bool m_AutoScroll = true;

public:
  OutputPanel();
  virtual ~OutputPanel() = default;

  virtual void ImGuiRender() override;
};