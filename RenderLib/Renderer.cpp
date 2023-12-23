#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "Application.h"
#include "Renderer.h"
#include "Log.h"

std::string postVert = R"(
#vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

#fragment
#version 330 core

// input and output
in vec2 TexCoord;
out vec4 FragColor;

// The input texture sampler
uniform sampler2D renderTexture;

uniform float time;


void main() {
    // Sample the color from the input render texture
    vec4 color = texture(renderTexture, TexCoord);

    // Output the sampled color as the final color
    float t = time;
    FragColor = color;
}
)";

GLFWwindow* Renderer::context = nullptr;
ptr<Mesh> Renderer::screenQuad = nullptr;
ptr<Material> Renderer::postProcessing = nullptr;
ptr<Camera> Renderer::mainCamera = nullptr;

Renderer::Renderer()
{
    renderTarget = std::make_shared<RenderTexture>();
    swapTarget = std::make_shared<RenderTexture>();
}

Renderer::~Renderer()
{
    Cleanup();
}

bool Renderer::Init(int targetWidth, int targetHeight)
{
    bool success = true;
    success &= renderTarget->Init(targetWidth, targetHeight, false);
    success &= swapTarget->Init(targetWidth, targetHeight, false);

    glViewport(0, 0, targetWidth, targetHeight);

    return success;
    //if (!context)
    //{
    //    Console::Error("Can't initialize Renderer without valid context");
    //    return false;
    //}

    //Renderer::context = context;

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); // Specify which faces to cull (in this case, back faces

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS); // Default depth comparison function

    //mainCamera = std::make_shared<Camera>(
    //    glm::vec3(0.0f, 0.0f, -5.0f),
    //    glm::vec3(0.0f, 1.0f, 0.0f),
    //    0.0f,
    //    0.0f
    //);
    //mainCamera->LookAt({ 0, 0, 0 });

    //screenQuad = Mesh::Quad();

    //auto winSize = GetContextSize();
    //renderTarget = std::make_shared<RenderTexture>();
    //renderTarget->Init(winSize.x, winSize.y, true);

    //auto defaultPost = std::make_shared<Shader>("Default Post");
    //defaultPost->InitFromSource(postVert);
    //postProcessing = std::make_shared<Material>("Default Post", defaultPost, renderTarget->GetTexture());

    //return true;
}

void Renderer::SetContext(GLFWwindow* context)
{
    Renderer::context = context;
}

void Renderer::Render()
{
    for each (RenderStep step in renderStack)
    {
        step.Execute();
    }

    renderTarget.swap(swapTarget);
}

void Renderer::PostProcess()
{
    float time = GetTime();

    glDisable(GL_DEPTH_TEST);
    postProcessing->SetTexture(renderTarget->GetTexture());
    postProcessing->Bind();
    postProcessing->SetUniform("time", time);
    screenQuad->Draw(GL_TRIANGLES);
}

void Renderer::Clear()
{
    renderTarget->Clear();
}

void Renderer::Cleanup()
{
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::SetRenderTarget(ptr<RenderTexture> source)
{
    this->renderTarget = source;
    this->postProcessing->SetTexture(source->GetTexture());
}

void Renderer::SetPostProcess(ptr<Material> post) 
{
    this->postProcessing = post;
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

const float Renderer::GetAspect()
{
    return GetWidth() / GetHeight();
}

float Renderer::GetTime()
{
    auto app = (Application*)glfwGetWindowUserPointer(context);
    return app->GetTime();
}

glm::vec2 Renderer::GetContextSize()
{
    Application* app = Application::GetInstance();
    return app->GetWindowSize();
}

ptr<Shader> Renderer::LoadShader(const std::string& name, const std::string & shaderPath)
{
    ptr<Shader> s = std::make_shared<Shader>(name);
    s->Init(shaderPath);

    return s;
}

// Function to draw a line between two glm::vec2 positions
void Renderer::DrawLine(const glm::vec3& a, const glm::vec3& b, GLenum usage) {
    std::vector<Vertex> vertices = {
        { a, { 0, 0 } },
        { b, {1, 1} }
    };

    std::vector<int> indices = {
        0, 1
    };

    Mesh l = Mesh(vertices, indices);
    l.Draw(GL_LINES);
}
    
//    // Vertex data for the line
//    float vertices[] = {
//        start.x, start.y,
//        end.x, end.y
//    };
//
//    // Create Vertex Array Object (VAO)
//    GLuint VAO;
//    glGenVertexArrays(1, &VAO);
//
//    // Create Vertex Buffer Object (VBO)
//    GLuint VBO;
//    glGenBuffers(1, &VBO);
//
//    // Bind the VAO
//    glBindVertexArray(VAO);
//
//    // Bind and initialize the VBO with vertex data
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, usage);
//
//    // Bind and configure the vertex buffer
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Draw the line
//    shader->Use();
//    glDrawArrays(GL_LINES, 0, 2);
//
//    // Unbind VAO and VBO
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // Delete VAO and VBO
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//}