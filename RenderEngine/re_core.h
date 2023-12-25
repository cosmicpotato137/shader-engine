#pragma once

#include "core.h"

#include <cmath>
#include <iostream>

#include "Application.h"
#include "ApplicationLayer.h"
#include "ComputeShader.h"
#include "Log.h"
#include "Mesh.h"
#include "RenderTexture.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#define SHADER_DIR \
  std::string("C:/Users/imacs/Documents/Code/Shader-Ideas/shaders")
#define RESOURCE_DIR \
  std::string("C:/Users/imacs/Documents/Code/Shader-Ideas/res")
#define TEXTURE_DIR RESOURCE_DIR + std::string("/textures")
