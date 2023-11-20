#pragma once 

#include "core.h"

#include "Shader.h"
#include "RenderTexture.h"
#include "RenderObject.h"
#include "Camera.h"

class Renderer {
	GLFWwindow* context;

	ptr<RenderTexture> renderTarget;
	ptr<Mesh> screenQuad;
	ptr<Material> postProcessing;
	
	ptr<Camera> mainCamera;

	std::vector<ptr<RenderObject>> scene;

public:
	Renderer();
	~Renderer();
	bool Init(GLFWwindow* context);
	void Render();
	void PostProcess();
	void Clear();
	void Cleanup();

	void SetClearColor(float r, float g, float b);
	void SetRenderTarget(ptr<RenderTexture> source);
	ptr<RenderTexture> GetRenderTarget() const { return renderTarget; }
	void SetPostProcess(ptr<Material> post);
	ptr<Material> GetPostProcess() const { return postProcessing; }

	ptr<Shader> LoadShader(const std::string& name, const std::string & shaderPath);

	void PushObject(ptr<RenderObject> mesh);

	float GetAspect() const;
	float GetTime() const;
	glm::vec2 GetContextSize() const;
	ptr<Camera> GetMainCamera() const { return mainCamera; }

private:
};


//void simpleMesh();