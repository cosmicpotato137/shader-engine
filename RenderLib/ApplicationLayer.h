#pragma once

#include "Event.h"
#include "Log.h"
#include "core.h"

#define HANDLE_EVENT_FN(type) virtual void Handle##type(type &e)

class ApplicationLayer {
  friend class Application;

  std::string name;

public:
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

protected:
  ApplicationLayer(const std::string &name) : name(name) {}

  // Called when adding layer to application
  virtual bool Init() { return true; }
  virtual void Cleanup() {}
};