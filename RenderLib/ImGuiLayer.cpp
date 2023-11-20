#include "ImGuiLayer.h"
#include "Application.h"


ImGuiLayer::ImGuiLayer(Application* app) : ApplicationLayer("imgui layer"), app(app) {

    // assign the render target texture to the compute shader's buffer
    glm::vec2 screenSize = app->GetWindowSize();
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    // init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;


    // set imgui style
    ImGui::StyleColorsDark();

    // Initialize ImGui for GLFW
    ImGui_ImplGlfw_InitForOpenGL(app->GetWindow(), true);

    // Initialize ImGui for OpenGL
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void ImGuiLayer::ImGuiBegin()
{
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::ImGuiEnd()
{
    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // viewport rendering
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::ImGuiRender()
{
    // styling and setup for the dockspace
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // styling for dockspace
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0, 0.0 });
    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0, 0, 0, 1 });

    window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // begin of dockspace window
    ImGui::Begin("Dock Space", (bool*)0, window_flags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();

    // reset styles
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}

void ImGuiLayer::HandleEvent(MouseButtonEvent& e)
{
    e.handled = ImGui::GetIO().WantCaptureMouse;
}