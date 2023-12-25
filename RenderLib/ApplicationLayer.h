#pragma once
#include "Event.h"
#include "Log.h"
#include "core.h"

#define HANDLE_EVENT_FN(type) \
  virtual void HandleEvent(type &e) {}

class ApplicationLayer {
  std::string name;

public:
  ApplicationLayer(const std::string &name) : name(name) {}

  // Update layer
  virtual void Update(double dt) {}

  // Render layer
  virtual void Render() {}

  // Render layer specific ui
  virtual void ImGuiRender() {}

  // Event handling
  HANDLE_EVENT_FN(Event)
  HANDLE_EVENT_FN(ScrollEvent)
  HANDLE_EVENT_FN(MouseButtonEvent)
  HANDLE_EVENT_FN(KeyboardEvent)
  HANDLE_EVENT_FN(CursorMovedEvent)
  HANDLE_EVENT_FN(WindowResizeEvent)
};

#undef HANDLE_EVENT_FN