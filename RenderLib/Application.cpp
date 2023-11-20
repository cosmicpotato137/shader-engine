#include "Application.h"

#include <functional>

static void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Application::Init(const std::string& winname)
{
    glfwSetErrorCallback(glfwErrorCallback);

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

    // enable vsync
    glfwSwapInterval(1);

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

    // push ui layer
    uiLayer = std::make_shared<ImGuiLayer>(this);
    PushLayer(uiLayer);

    Start();
    return true;
}

void Application::PushLayer(ptr<ApplicationLayer> layer)
{
    layers.push_back(layer);
}

void Application::PopLayer()
{
    layers.pop_back();
}

void Application::Run()
{
    double lastTime = 0;

    while (!glfwWindowShouldClose(window)) {
        // Process input events (e.g., keyboard and mouse)
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // handle application updates
        for (ptr<ApplicationLayer> l : layers)
        {
            l->Update(GetTime() - lastTime);
        }

        lastTime = GetTime();

        // render graphics
        for (ptr<ApplicationLayer> l : layers)
        {
            l->Render();
        }

        // render ui
        uiLayer->ImGuiBegin();
        for (ptr<ApplicationLayer> l : layers)
        {
            l->ImGuiRender();
        }
        uiLayer->ImGuiEnd();

        glfwSwapBuffers(window);
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

bool Application::OnEvent(event_types e)
{
    bool handled = false;
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        handled = std::visit(EventVisitor(layers[i]), e);

        if (handled)
            break;
    }

    return handled;
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
        MouseButtonEvent me = { false, button, action, mods };
        if (!app->OnEvent(me))
            app->OnMouseButton(button, action, mods);
    }
}

void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        CursorMovedEvent me = { false, xpos, ypos };
        if (!app->OnEvent(me))
            app->OnMousePos(xpos, ypos);
    }
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        ScrollEvent se = { false, xoffset, yoffset };
        if (!app->OnEvent(se))
            app->OnScroll(xoffset, yoffset);
    }
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        KeyboardEvent ke = { false, key, scancode, action, mods };
        if (!app->OnEvent(ke))
            app->OnKey(key, scancode, action, mods);
    }
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        WindowResizeEvent we = { false, width, height };
        if (!app->OnEvent(we))
            app->OnWindowResize(width, height);
    }
}