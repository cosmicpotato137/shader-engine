#include "Application.h"

#include <functional>

bool Application::Init(const std::string& winname)
{
    // Initialize GLFW
    if (!glfwInit()) {
        Console::Error("Failed to initialize GLFW");
        return false;
    }

    //Configure GLFW (optional)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    window = glfwCreateWindow(800, 600, winname.c_str(), nullptr, nullptr);
    if (!window) {
        Console::Error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }


    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize glad
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        Console::Error("Failed to initialize GLEW");
        return false;
    }

    // Print OpenGL version
    std::string glVersion((char*)glGetString(GL_VERSION));
    Console::Log("OpenGL Version: %s", glVersion.c_str());

    // set the window user pointer
    glfwSetWindowUserPointer(window, this);

    // set window size
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    // set callbacks
    glfwSetKeyCallback(window, Application::KeyCallback);
    glfwSetMouseButtonCallback(window, Application::MouseButtonCallback);
    glfwSetCursorPosCallback(window, Application::CursorPosCallback);
    glfwSetWindowSizeCallback(window, Application::WindowSizeCallback);
    glfwSetScrollCallback(window, Application::ScrollCallback);

    Start();
    return true;
}

void Application::Run()
{
    double lastTime = 0;

    while (!glfwWindowShouldClose(window)) {
        // Process input events (e.g., keyboard and mouse)
        glfwPollEvents();

        lastTime = GetTime();
        Update(GetTime() - lastTime);
    }
}

void Application::Cleanup()
{
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

glm::vec2 Application::GetCursorPosition()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

void Application::OnWindowResize(int width, int height)
{
    this->screenWidth = width;
    this->screenHeight = height;
    glViewport(0, 0, width, height);
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->OnMouseButton(button, action, mods);
    }
}

void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->OnMousePos(xpos, ypos);
    }
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->OnScroll(xoffset, yoffset);
    }
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->OnKey(key, scancode, action, mods);
    }
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->OnWindowResize(width, height);
    }
}