#pragma once

#include "core.h"

#include "Camera.h"
#include "RenderObject.h"
#include "RenderTexture.h"
#include "Shader.h"

#include <functional>

class RenderStep {
  std::function<void()> renderFunction;

public:
  RenderStep(std::function<void()> renderFunction)
      : renderFunction(renderFunction) {}

  // Method to execute the lambda function
  void Execute() {
    if (renderFunction) {
      renderFunction();
    }
  }
};

class Renderer {
  static GLFWwindow *context;

  static ptr<Mesh> screenQuad;
  static ptr<Material> postProcessing;

  static ptr<Camera> mainCamera;

  ptr<RenderTexture> renderTarget;
  ptr<RenderTexture> swapTarget;
  std::vector<RenderStep> renderStack;

public:
  Renderer();
  ~Renderer();

  // Initialize renderer with render target width and height
  bool Init(int targetWidth, int targetHeight);

  // Set the context (glfw window) for the renderer
  static void SetContext(GLFWwindow *context);

  void Render();
  void PostProcess();
  void Clear();
  void Cleanup();

  const float GetWidth() { return renderTarget->GetWidth(); }
  const float GetHeight() { return renderTarget->GetHeight(); }
  const glm::vec2 GetSize() { return renderTarget->GetSize(); }
  const float GetAspect();

  static void SetClearColor(float r, float g, float b, float a = 1);
  static void SetViewport(int x, int y, int width, int height);
  static float GetTime();
  static glm::vec2 GetContextSize();

  // Render target getter
  ptr<RenderTexture> GetRenderTarget() const { return renderTarget; }

  // Swap target getter and setter

  void SetRenderTarget(ptr<RenderTexture> source);
  ptr<RenderTexture> GetSwapTarget() const { return swapTarget; }

  void SetPostProcess(ptr<Material> post);
  ptr<Material> GetPostProcess() const { return postProcessing; }
  ptr<Camera> GetMainCamera() const { return mainCamera; }

  ptr<Shader>
  LoadShader(const std::string &name, const std::string &shaderPath);

  static void DrawLine(
      const glm::vec3 &a, const glm::vec3 &b, GLenum usage = GL_STATIC_DRAW);

private:
};
