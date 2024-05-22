#pragma once
#include "Core/core.h"
#include <variant>

struct Event {
  bool handled = false;
};

struct CursorMovedEvent : public Event {
  double xPos, yPos;
};

struct MouseButtonEvent : public Event {
  GLint button;
  GLint action;
  GLint mods;
};

struct ScrollEvent : public Event {
  double xOffset, yOffset;
};

struct KeyboardEvent : public Event {
  GLint key;
  GLint scancode;
  GLint action;
  GLint mods;
};

struct WindowResizeEvent : public Event {
  int width, height;
};

struct WindowMoveEvent : public Event {
  int xPos, yPos;
};

typedef std::variant<
    std::reference_wrapper<CursorMovedEvent>,
    std::reference_wrapper<MouseButtonEvent>,
    std::reference_wrapper<ScrollEvent>,
    std::reference_wrapper<WindowResizeEvent>,
    std::reference_wrapper<WindowMoveEvent>,
    std::reference_wrapper<KeyboardEvent>>
    event_types;

#define HANDLE_EVENT_FN(type) virtual void Handle##type(type &e)
