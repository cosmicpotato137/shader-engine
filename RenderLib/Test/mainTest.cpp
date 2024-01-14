#include "gtest/gtest.h"
#include "ApplicationTest.h"
#include "ComputeBuffer1DTest.h"
#include "CameraTest.h"
#include "ComputeShaderTest.h"
#include "TextureTest.h"
#include "RendererTest.h"
#include "RenderTextureTest.h"
#include "ShaderTest.h"
#include "MaterialTest.h"
#include "ShaderUniformTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}