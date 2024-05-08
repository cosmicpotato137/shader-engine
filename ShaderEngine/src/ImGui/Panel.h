#pragma once

#include <string>
#include "imgui.h"

class Panel {
protected:
  std::string m_Name = "";
  ImVec2 m_Size;

public:
  Panel(const std::string &name) : m_Name(name){};
  Panel() = default;
  virtual ~Panel() = default;

  virtual void ImGuiRender() = 0;
  virtual void OnResize() {}

  ImVec2 GetSize() const { return m_Size; }
};