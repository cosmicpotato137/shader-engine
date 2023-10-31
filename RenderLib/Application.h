#pragma once

#include "core.h"
#include "Log.h"
#include "ApplicationLayer.h"

class Application {
protected:
    GLFWwindow* window;
    int screenWidth, screenHeight;

    std::vector<ptr<ApplicationLayer>> layers;

public:
    Application() : window(nullptr), screenWidth(0), screenHeight(0) {}
    ~Application() { Cleanup(); }

    // Initialize GLFW and GLEW
    bool Init(const std::string& winname);

    // add application layer
    void PushLayer(ptr<ApplicationLayer> layer);
    // remove application layer
    void PopLayer();

    // Main program loop
    void Run();
    void SetWindowShouldClose(bool close) { glfwSetWindowShouldClose(window, close); }

    // Clean up GLFW and release resources
    virtual void Cleanup();

    // get the glfw context
    GLFWwindow* GetWindow() const { return window; }
    // get the aspect ratio of the window
    float GetAspect() const { return (float)screenWidth / (float)screenHeight; }
    // get window size
    glm::vec2 GetWindowSize() { return { screenWidth, screenHeight }; }

    // get current time in app
    double GetTime() { return glfwGetTime(); }
    // get the mouse position
    glm::vec2 GetCursorPosition();

    // generic event callback
    virtual bool OnEvent(event_types e);

    // key callback
    virtual void OnKey(int key, int scancode, int action, int mods) { Console::Log("base keyboard"); }
    // mouse callback
    virtual void OnMousePos(double xpos, double ypos) { Console::Log("base mouse pos"); }
    // scroll callback
    virtual void OnScroll(double xoffset, double yoffset) {}
    // mouse button callback
    virtual void OnMouseButton(int button, int action, int mods) { Console::Log("base mouse button"); }
    // window resize callback
    virtual void OnWindowResize(int width, int height);

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void WindowSizeCallback(GLFWwindow* window, int width, int height);

protected:
    // override this to add startup functionality
    virtual void Start() = 0;

    // override this to add functionality to the program loop
    virtual void Update(double dt) = 0;
};

#define EVENT_FN(type) bool operator()(type& e) { layer->HandleEvent(e); return e.handled; }

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