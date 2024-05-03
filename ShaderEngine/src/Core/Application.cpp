#include "Application.h"
#include "Events/EventVisitor.h"
#include "Renderer/Renderer.h"

#include <functional>

Application *Application::s_Instance = nullptr;

static void glfwErrorCallback(int error, const char *description) {
  Console::Error("GLFW Error %d: %s\n", error, description);
}

Application *Application::GetInstance() {
  Console::Assert(
      s_Instance,
      "Application instance not created. This is may be due to "
      "CreateApplication not being declared.");
  return s_Instance;
}

void Application::DestroyInstance() {
  if (s_Instance) {
    s_Instance->Shutdown();
    delete s_Instance;
    s_Instance = nullptr;
  }
}

bool Application::Init() {
  // Set working directory here
  if (!properties.WorkingDirectory.empty())
    std::filesystem::current_path(properties.WorkingDirectory);

  glfwSetErrorCallback(glfwErrorCallback);

  // Initialize GLFW
  if (!glfwInit()) {
    Console::Error("Failed to initialize GLFW");
    return false;
  }

  // Configure GLFW (optional)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFW window
  window = glfwCreateWindow(
      properties.Width,
      properties.Height,
      properties.Name.c_str(),
      nullptr,
      nullptr);
  if (!window) {
    Console::Error("Failed to create GLFW window");
    glfwTerminate();
    return false;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Enable vsync
  glfwSwapInterval(1);

  // Initialize renderer
  if (!Renderer::InitOpenGL())
    return false;
  Renderer::SetContext(window);

  // Set the window user pointer
  glfwSetWindowUserPointer(window, this);

  // Set callbacks
  glfwSetKeyCallback(window, Application::KeyCallback);
  glfwSetMouseButtonCallback(window, Application::MouseButtonCallback);
  glfwSetCursorPosCallback(window, Application::CursorPosCallback);
  glfwSetWindowSizeCallback(window, Application::WindowSizeCallback);
  glfwSetScrollCallback(window, Application::ScrollCallback);

  // Push ui layer
  uiLayer = std::make_shared<ImGuiLayer>();
  PushLayer(uiLayer);

  // Push other layers (defined in subclass)
  InitLayerStack();

  initialized = true;
  return true;
}

void Application::PushLayer(ptr<ApplicationLayer> layer) {
  layer->OnAttach();
  layers.push_back(layer);
}

void Application::PopLayer() {
  layers.back()->OnDetach();
  layers.pop_back();
}

void Application::Run() {
  double lastTime = 0;

  while (!glfwWindowShouldClose(window)) {
    // Process input events (e.g., keyboard and mouse)
    glfwPollEvents();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Handle application updates
    for (ptr<ApplicationLayer> l : layers) {
      l->Update(GetTime() - lastTime);
    }

    lastTime = GetTime();

    // Render graphics
    for (ptr<ApplicationLayer> l : layers) {
      l->Render();
    }

    // Render ui
    uiLayer->ImGuiBegin();
    for (ptr<ApplicationLayer> l : layers) {
      l->ImGuiRender();
    }
    uiLayer->ImGuiEnd();

    glfwSwapBuffers(window);
  }
}

void Application::SetWindowShouldClose(bool close) {
  glfwSetWindowShouldClose(window, close);
}

void Application::Shutdown() {
  if (!initialized)
    return;
  initialized = false;

  // Cleanup imgui layers
  for (ptr<ApplicationLayer> l : layers) {
    l->OnDetach();
  }
  layers.clear();
  uiLayer.reset();
  uiLayer = nullptr;

  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;  // Nullify the window pointer
  }

  glfwTerminate();
}

glm::vec2 Application::GetCursorPosition() {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return glm::vec2(xpos, ypos);
}

bool Application::OnEvent(event_types e) {
  bool handled = false;
  for (int i = layers.size() - 1; i >= 0; i--) {
    handled = std::visit(EventVisitor(layers[i]), e);

    if (handled)
      break;
  }

  return handled;
}

void Application::OnWindowResize(int width, int height) {
  properties.Width = width;
  properties.Height = height;
  // todo: do we need this?
  glViewport(0, 0, width, height);
}

void Application::MouseButtonCallback(
    GLFWwindow *window, int button, int action, int mods) {
  Application *app =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (app) {
    MouseButtonEvent me = {false, button, action, mods};
    if (!app->OnEvent(me))
      app->OnMouseButton(button, action, mods);
  }
}

void Application::CursorPosCallback(
    GLFWwindow *window, double xpos, double ypos) {
  Application *app =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (app) {
    CursorMovedEvent me = {false, xpos, ypos};
    if (!app->OnEvent(me))
      app->OnMousePos(xpos, ypos);
  }
}

void Application::ScrollCallback(
    GLFWwindow *window, double xoffset, double yoffset) {
  Application *app =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (app) {
    ScrollEvent se = {false, xoffset, yoffset};
    if (!app->OnEvent(se))
      app->OnScroll(xoffset, yoffset);
  }
}

void Application::KeyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
  Application *app =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (app) {
    KeyboardEvent ke = {false, key, scancode, action, mods};
    if (!app->OnEvent(ke))
      app->OnKey(key, scancode, action, mods);
  }
}

void Application::WindowSizeCallback(
    GLFWwindow *window, int width, int height) {
  Application *app =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (app) {
    WindowResizeEvent we = {false, width, height};
    if (!app->OnEvent(we))
      app->OnWindowResize(width, height);
  }
}