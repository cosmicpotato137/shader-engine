#pragma once

#include "core.h"
#include "Log.h"

class Application {
protected:
    GLFWwindow* window;
    int width, height;
public:
    Application() : window(nullptr), width(0), height(0) {}
    ~Application() { Cleanup(); }

    // Initialize GLFW and GLEW
    bool Init(const std::string& winname);

    // Main program loop
    void Run();

    // Clean up GLFW and release resources
    virtual void Cleanup();

    // get the glfw context
    GLFWwindow* GetWindow() const { return window; }
    // get the aspect ratio of the window
    float GetAspect() const { 
        return (float)width / (float)height; 
    }

    // get window size
    glm::vec2 GetWindowSize() { 
        return { width, height };
    }

    // get current time in app
    double GetTime() { return glfwGetTime(); }
    // get the mouse position
    glm::vec2 GetCursorPosition();

    // key callback
    virtual void OnKey(int key, int scancode, int action, int mods) { Console::Log("base keyboard"); }
    // mouse callback
    virtual void OnMousePos(double xpos, double ypos) { Console::Log("base mouse pos"); };
    // mouse button callback
    virtual void OnScroll(double xoffset, double yoffset) {}
    virtual void OnMouseButton(int button, int action, int mods) { Console::Log("base mouse button"); };
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