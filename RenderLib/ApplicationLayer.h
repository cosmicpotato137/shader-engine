#pragma once
#include "core.h"
#include "Log.h"
#include "Event.h"

#define HANDLE_EVENT_FN(type) virtual void HandleEvent(type& e) {}

class ApplicationLayer {
	std::string name;
public:
	ApplicationLayer(const std::string& name) : name(name) {}

	virtual void Update(double dt) {}
	virtual void Render() {}
	

	HANDLE_EVENT_FN(Event)
	HANDLE_EVENT_FN(ScrollEvent)
	HANDLE_EVENT_FN(MouseButtonEvent)
	HANDLE_EVENT_FN(KeyboardEvent)
	HANDLE_EVENT_FN(CursorMovedEvent)
	HANDLE_EVENT_FN(WindowResizeEvent)
};

#undef HANDLE_EVENT_FN