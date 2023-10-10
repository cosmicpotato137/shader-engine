#pragma once 

#include "core.h"

#include "Shader.h"
#include "RenderTexture.h"
#include "RenderObject.h"
#include "Camera.h"

class Renderer {
	static Renderer* s_Instance;

	GLFWwindow* context;
	ptr<RenderTexture> renderTarget;
	ptr<Mesh> screenQuad;
	ptr<Material> postProcessing;
	
	ptr<Camera> mainCamera;

	std::vector<ptr<RenderObject>> scene;

public:
	Renderer GetInstance()
	{
		if (s_Instance)
			return *s_Instance;
		else
			return Renderer();
	}

	Renderer();
	~Renderer();
	bool Init(GLFWwindow* context);
	void Render();
	void Cleanup();

	void SetClearColor(int r, int g, int b);
	void SetRenderTarget(ptr<RenderTexture> source);
	void SetPostProcess(ptr<Material> post);

	ptr<Shader> LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void SetPostMaterial(ptr<Material> postProcess) { this->postProcessing = postProcess; }

	void PushObject(ptr<RenderObject> mesh);

	float GetAspect() const;
	float GetTime() const;
	glm::vec2 GetContextSize() const;
	ptr<Camera> GetMainCamera() const { return mainCamera; }

private:
};


//void simpleMesh();