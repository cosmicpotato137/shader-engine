#pragma once 

#include "core.h"

#include "Shader.h"
#include "RenderTexture.h"
#include "RenderObject.h"
#include "Camera.h"

class Renderer {
	static GLFWwindow* context;

	static ptr<RenderTexture> renderTarget;
	static ptr<Mesh> screenQuad;
	static ptr<Material> postProcessing;
	
	static ptr<Camera> mainCamera;

	std::vector<ptr<RenderObject>> scene;

public:
	Renderer();
	~Renderer();

	static bool Init(GLFWwindow* context);

	void Render();
	void PostProcess();
	void Clear();
	void Cleanup();

	static void SetClearColor(float r, float g, float b);
	static void SetViewport(int x, int y, int width, int height);
	static float GetAspect();
	static float GetTime();
	static glm::vec2 GetContextSize();

	void SetRenderTarget(ptr<RenderTexture> source);
	ptr<RenderTexture> GetRenderTarget() const { return renderTarget; }
	void SetPostProcess(ptr<Material> post);
	ptr<Material> GetPostProcess() const { return postProcessing; }
	ptr<Camera> GetMainCamera() const { return mainCamera; }

	ptr<Shader> LoadShader(const std::string& name, const std::string & shaderPath);

	void PushObject(ptr<RenderObject> mesh);


	static void DrawLine(const glm::vec2& a, const glm::vec2& b);

private:
};
