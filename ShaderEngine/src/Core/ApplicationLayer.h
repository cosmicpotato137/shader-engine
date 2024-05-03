#pragma once

#include "Events/Event.h"
#include "Core/Log.h"
#include "Core/core.h"

#define HANDLE_EVENT_FN(type) virtual void Handle##type(type &e)

class ApplicationLayer {
  friend class Application;

  std::string name;

protected:
  ApplicationLayer(const std::string &name) : name(name) {}

public:
  // Called when layer is attached to or
  // detached from application
  virtual bool OnAttach() = 0;
  virtual void OnDetach() = 0;

  // Update layer
  virtual void Update(double dt) {}

  // Render layer
  virtual void Render() {}

  // Render layer specific ui
  virtual void ImGuiRender() {}

  // Event handling
  HANDLE_EVENT_FN(Event) {
    Console::Log("Generic event handled, something is probably wrong.");
  }
  HANDLE_EVENT_FN(ScrollEvent) {}
  HANDLE_EVENT_FN(MouseButtonEvent) {}
  HANDLE_EVENT_FN(KeyboardEvent) {}
  HANDLE_EVENT_FN(CursorMovedEvent) {}
  HANDLE_EVENT_FN(WindowResizeEvent) {}
};