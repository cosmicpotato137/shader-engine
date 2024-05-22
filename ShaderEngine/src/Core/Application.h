#pragma once

#include "ApplicationLayer.h"
#include "GUI/ImGuiLayer.h"
#include "Core/Log.h"
#include "Core/core.h"

struct ApplicationCommandLineArgs {
  int Count = 0;
  char **Args = nullptr;

  const char *operator[](int index) const {
    Console::Assert(index < Count, "Index out of range");
    return Args[index];
  }
};

struct ApplicationProps {
  std::string Name = "Application";
  std::string WorkingDirectory = ROOT_DIR;
  int Width = 1280, Height = 720;
  ApplicationCommandLineArgs Args = {0, nullptr};
};

class Application {
  friend class ApplicationTest;

  // Essentially a template static function
  extern friend Application *
  CreateApplication(const ApplicationCommandLineArgs &props);

  static Application *s_Instance;
  GLFWwindow *window = nullptr;
  bool initialized = false;

  ApplicationProps properties;
  std::unordered_map<int, bool> m_KeyState;
  std::unordered_map<int, bool> m_MouseButtonState;
  glm::vec2 m_CursorPosition;
  glm::vec2 m_WindowPosition;

  std::vector<ptr<ApplicationLayer>> layers;
  ptr<ImGuiLayer> uiLayer;

protected:
  Application(const ApplicationProps &props = ApplicationProps());
  ~Application() {}

public:
  // Get the application instance
  static Application *GetInstance();
  static void DestroyInstance();

  // Initialize GLFW and GLEW
  bool Init();
  virtual void InitLayerStack() = 0;

  // Add application layer
  void PushLayer(ptr<ApplicationLayer> layer);

  // Remove application layer
  void PopLayer();

  // Main program loop
  void Run();

  // Check if window should close
  void SetWindowShouldClose(bool close);

  // Clean up GLFW and release resources
  virtual void Shutdown();

  // Get properties
  ApplicationProps GetProperties() { return properties; }

  // Get the glfw context
  GLFWwindow *GetWindow() const { return window; }

  // Get the aspect ratio of the window
  float GetAspect() const {
    return (float)properties.Width / (float)properties.Height;
  }

  // Get window size
  glm::vec2 GetWindowSize() { return {properties.Width, properties.Height}; }

  // Get current time in app
  double GetTime() { return glfwGetTime(); }

  // Get the working directory
  std::string GetWorkingDirectory() { return properties.WorkingDirectory; }

  // Get key state
  bool GetKey(int key);

  // Get mouse button state
  bool GetMouseButton(int button);

  // Get the mouse position
  glm::vec2 GetCursorPosition();
  glm::vec2 GetCursorScreenPosition();
  glm::vec2 GetWindowPosition();

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
  // Window move callback
  virtual void OnWindowMove(int xpos, int ypos);

  // Window callbacks for glfw
  static void
  MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
  static void
  ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
  static void
  KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void WindowSizeCallback(GLFWwindow *window, int width, int height);
  static void WindowPosCallback(GLFWwindow *window, int xpos, int ypos);
};
