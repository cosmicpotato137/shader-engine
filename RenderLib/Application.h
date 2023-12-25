#pragma once

#include "ApplicationLayer.h"
#include "ImGuiLayer.h"
#include "Log.h"
#include "core.h"

class Application {
protected:
  static Application *s_Instance;

  GLFWwindow *window;
  int screenWidth, screenHeight;

  std::vector<ptr<ApplicationLayer>> layers;
  ptr<ImGuiLayer> uiLayer;

public:
  Application() : window(nullptr), screenWidth(0), screenHeight(0) {
    // Only one application instance allowed
    if (s_Instance) {
      Console::Error("only one application instance allowed");
      this->~Application();
    }
    s_Instance = this;
  }

  ~Application() { Cleanup(); }

  // Get the application instance
  static Application *GetInstance() { return s_Instance; }

  // Initialize GLFW and GLEW
  bool Init(const std::string &winname);

  // Add application layer
  void PushLayer(ptr<ApplicationLayer> layer);

  // Remove application layer
  void PopLayer();

  // Main program loop
  void Run();

  // Check if window should close
  void SetWindowShouldClose(bool close) {
    glfwSetWindowShouldClose(window, close);
  }

  // Clean up GLFW and release resources
  virtual void Cleanup();

  // Get the glfw context
  GLFWwindow *GetWindow() const { return window; }

  // Get the aspect ratio of the window
  float GetAspect() const { return (float)screenWidth / (float)screenHeight; }

  // Get window size
  glm::vec2 GetWindowSize() { return {screenWidth, screenHeight}; }

  // Get current time in app
  double GetTime() { return glfwGetTime(); }

  // Get the mouse position
  glm::vec2 GetCursorPosition();

  // Generic event callback
  virtual bool OnEvent(event_types e);

  // Key callback
  virtual void OnKey(int key, int scancode, int action, int mods) {}
  // Mouse callback
  virtual void OnMousePos(double xpos, double ypos) {}
  // Scroll callback
  virtual void OnScroll(double xoffset, double yoffset) {}
  // Mouse button callback
  virtual void OnMouseButton(int button, int action, int mods) {}
  // Window resize callback
  virtual void OnWindowResize(int width, int height);

  // Window callbacks for glfw
  static void MouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
  static void ScrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void WindowSizeCallback(GLFWwindow *window, int width, int height);
};

// Event visitor for application layers
#define EVENT_FN(type)       \
  bool operator()(type &e) { \
    layer->HandleEvent(e);   \
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
};

#undef EVENT_FN