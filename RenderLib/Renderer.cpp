#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Application.h"
#include "Log.h"
#include "Renderer.h"
#include "Mesh.h"

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

// Input and output
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

GLFWwindow *Renderer::context = nullptr;

Renderer::Renderer() {
  renderTarget = std::make_shared<RenderTexture>();
  swapTarget = std::make_shared<RenderTexture>();
}

Renderer::~Renderer() { Cleanup(); }

bool Renderer::Init(int targetWidth, int targetHeight) {
  bool success = true;
  success &= renderTarget->Init(targetWidth, targetHeight, false);
  success &= swapTarget->Init(targetWidth, targetHeight, false);

  glViewport(0, 0, targetWidth, targetHeight);

  return success;
}

void Renderer::SetContext(GLFWwindow *context) { Renderer::context = context; }

void Renderer::Render(bool clear) {
  renderTarget->BeginRender(clear);
  for (RenderStep &step : renderStack) {
    step.Execute();
  }
  renderTarget->EndRender();

  renderTarget.swap(swapTarget);
}

void Renderer::Clear() { renderTarget->Clear(); }

void Renderer::Cleanup() {
  renderTarget->Cleanup();
  swapTarget->Cleanup();
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Renderer::SetRenderTarget(ptr<RenderTexture> source) {
  this->renderTarget = source;
}

void Renderer::SetViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

const float Renderer::GetAspect() { return GetWidth() / GetHeight(); }

// Function to draw a line between two glm::vec2 positions
void Renderer::DrawLine(const glm::vec3 &a, const glm::vec3 &b, GLenum usage) {
  std::vector<Vertex> vertices = {{a, {0, 0}}, {b, {1, 1}}};

  std::vector<int> indices = {0, 1};

  Mesh l = Mesh(vertices, indices);
  l.Draw(GL_LINES);
}