#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "Application.h"
#include "Renderer.h"
#include "Log.h"

Renderer* Renderer::s_Instance = nullptr;

std::string postFrag = R"(
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

std::string postVert = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

Renderer::Renderer()
    : context(nullptr), renderTarget(nullptr)
{
    if (!s_Instance)
        s_Instance = this;
    else
    {
        Console::Log("a renderer already exists");
        delete (this);
    }
}

Renderer::~Renderer()
{
    Cleanup();
}

bool Renderer::Init(GLFWwindow* context)
{
    if (!context)
    {
        Console::Error("Can't initialize Renderer without valid context");
        return false;
    }

    this->context = context;
    if (glfwGetCurrentContext() != context)
        glfwMakeContextCurrent(context);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Specify which faces to cull (in this case, back faces

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Default depth comparison function


    mainCamera = std::make_shared<Camera>(
        glm::vec3(0.0f, 0.0f, -5.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.0f,
        0.0f
    );
    mainCamera->LookAt({ 0, 0, 0 });

    screenQuad = Mesh::Quad();

    auto winSize = GetContextSize();
    renderTarget = std::make_shared<RenderTexture>(winSize.x, winSize.y);
    renderTarget->Init();

    auto defaultPost = std::make_shared<Shader>("Default Post");
    defaultPost->InitFromSource(postVert, postFrag);
    postProcessing = std::make_shared<Material>("Default Post", defaultPost, renderTarget->GetTexture());

    return true;
}

void Renderer::Render()
{
    // render to a render texture first so we can do post processing!
    float time = GetTime();

    renderTarget->BeginRender();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto object : scene)
    {
        glm::mat4 model = object->transform->GetWorldMatrix();
        object->material->SetUniform("model", model);
        glm::mat4 view = mainCamera->GetViewMatrix();
        glm::mat4 proj = mainCamera->GetProjectionMatrix(GetAspect());
        object->material->SetUniform("viewProjection", proj * view);
        object->material->SetUniform("time", time);
        object->material->Bind();
        object->mesh->Draw();
    }

    renderTarget->EndRender();
}

void Renderer::PostProcess()
{
    float time = GetTime();

    glDisable(GL_DEPTH_TEST);
    postProcessing->Bind();
    postProcessing->SetUniform("time", time);
    screenQuad->Draw();

    glfwSwapBuffers(context);
}

void Renderer::Cleanup()
{
    screenQuad->Cleanup();
}

void Renderer::SetClearColor(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
}

void Renderer::SetRenderTarget(ptr<RenderTexture> source)
{
    this->renderTarget = source;
    this->postProcessing->SetTexture(source->GetTexture());
}

void Renderer::SetPostProcess(ptr<Material> post) 
{
    post->SetTexture(renderTarget->GetTexture());
    this->postProcessing = post;
}

float Renderer::GetAspect() const
{
    auto app = (Application*)glfwGetWindowUserPointer(context);
    return app->GetAspect();
}

float Renderer::GetTime() const
{
    auto app = (Application*)glfwGetWindowUserPointer(context);
    return app->GetTime();
}

glm::vec2 Renderer::GetContextSize() const
{
    auto app = (Application*)glfwGetWindowUserPointer(context);
    return app->GetWindowSize();
}

void Renderer::PushObject(ptr<RenderObject> mesh)
{
    scene.push_back(mesh);
}

ptr<Shader> Renderer::LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    ptr<Shader> s = std::make_shared<Shader>(name);
    s->Init(vertexShaderPath, fragmentShaderPath);

    return s;
}
