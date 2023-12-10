#pragma once

#include "ComputeShader.h"
#include "RenderTexture.h"

class ComputeObject
{
	std::string name;
	ptr<ComputeShader> shader;
	char* shaderPath = new char[256];
	ptr<RenderTexture> imageIn;
	ptr<RenderTexture> imageOut;

	bool pause = false, reset = false, step = false;
public:
	ComputeObject() = delete;
	ComputeObject(const std::string& name, const std::string& shaderPath, ptr<RenderTexture> imageIn, ptr<RenderTexture> imageOut);

	void SetShader(const std::string& shaderPath);
	ptr<ComputeShader> GetShader() const { return shader; }
	void RecompileShader();

	void Step();
	void ToggleStop();
	void Start();
	void Stop();

	void Render();
	void ImGuiRender();

private:
	void ShaderInfoWindow();
};