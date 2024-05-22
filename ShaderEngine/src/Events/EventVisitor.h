#pragma once
#include "Core/ApplicationLayer.h"

// Event visitor for application layers
// Remember to name your event handling
// functions with the same name as the event
#define EVENT_FN(type)       \
  bool operator()(type &e) { \
    layer->Handle##type(e);  \
    return e.handled;        \
  }

struct EventVisitor {
  ptr<ApplicationLayer> layer;

  EventVisitor(ptr<ApplicationLayer> layer) : layer(layer) {}

  EVENT_FN(CursorMovedEvent)
  EVENT_FN(MouseButtonEvent)
  EVENT_FN(WindowResizeEvent)
  EVENT_FN(ScrollEvent)
  EVENT_FN(KeyboardEvent)
  EVENT_FN(WindowMoveEvent)
};

#undef EVENT_FN