#include "Application.h"
#include "Camera.h"
#include "Renderer.h"
#include "RenderTexture.h"
#include "ComputeShader.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "re_core.h"

class ImGuiLayer : public ApplicationLayer {
    Application* app;
    bool showDemo = true;

public:
    ImGuiLayer(Application* app) : ApplicationLayer("imgui layer"), app(app) {

        // assign the render target texture to the compute shader's buffer
        glm::vec2 screenSize = app->GetWindowSize();
        int screenWidth = screenSize.x;
        int screenHeight = screenSize.y;

        // init imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // set imgui style
        ImGui::StyleColorsDark();

        // Initialize ImGui for GLFW
        ImGui_ImplGlfw_InitForOpenGL(app->GetWindow(), true);

        // Initialize ImGui for OpenGL
        ImGui_ImplOpenGL3_Init("#version 460 core");
    }

    void Update(double dt) {
    }

    void Render() override {
        // Start a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow(&showDemo);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0, 0.0 });
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0, 0, 0, 0 });

        ImGui::Begin("Dock Space");

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(3);

        // Create an ImGui window
        ImGui::Begin("ImGui Window");

        // Add ImGui widgets, e.g., buttons, sliders, etc.
        ImGui::Text("Hello, ImGui!");
        if (ImGui::Button("Click Me")) {
            Console::Log("Hello world!");
        }

        // End the ImGui window
        ImGui::End();

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // viewport rendering
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void HandleEvent(MouseButtonEvent& e) override
    {
        e.handled = ImGui::GetIO().WantCaptureMouse;
    }

    void HandleEvent(WindowResizeEvent& e) override
    {

    }

    void HandleEvent(KeyboardEvent& e) override
    {
        if (e.key == GLFW_KEY_SPACE)
        {
            showDemo != showDemo;
            e.handled = true;
        }
    }
};