#pragma once

#include <string>
#include "imgui.h"
#include "Renderer/Renderer.h"
#include "Core/Serial.h"

class Panel {
protected:
  std::string m_Name;
  ImVec2 m_Size;

public:
  Panel(const std::string &name = "Panel") : m_Name(name){};
  Panel(const Panel &other) = default;
  virtual ~Panel() = default;

  virtual void ImGuiRender() = 0;
  virtual void Update(double dt) {}
  virtual void Render(const Renderer &ren) {}
  virtual void OnResize() {}

  std::string GetName() const { return m_Name; }
  ImVec2 GetSize() const { return m_Size; }

  Panel &operator=(const Panel &other);

  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & m_Name;
    ar & m_Size;
  }
};