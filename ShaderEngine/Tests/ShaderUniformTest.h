#pragma once

#include "gtest/gtest.h"
#include "Renderer/ShaderUniform.h"

#include <filesystem>

void SerializeUniformTestWrapper(const Uniform &uniform) {
  auto current_path = std::filesystem::current_path();
  auto test_path =
      current_path / "bin" / "Debug" / "data" / "test_uniform.dat";

  Serial::Save(uniform, test_path.string());
  auto loaded_uniform = Serial::Load<Uniform>(test_path.string());

  EXPECT_EQ(uniform.GetName(), loaded_uniform->GetName());
  EXPECT_EQ(uniform.GetValue(), loaded_uniform->GetValue());
  EXPECT_EQ(uniform.GetType(), loaded_uniform->GetType());
  EXPECT_EQ(uniform.GetHide(), loaded_uniform->GetHide());
}

class ShaderUniformTest : public ::testing::Test {
protected:
  virtual void SetUp() {}

  virtual void TearDown() {}

  void TestUniform() {
    std::string name = "test";
    int location = 0;
    uniform_types value = 1;
    UniformType type = UniformType::Int;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    EXPECT_EQ(uniform.GetName(), name);
    EXPECT_EQ(uniform.GetLocation(), location);
    EXPECT_EQ(uniform.GetValue(), value);
    EXPECT_EQ(uniform.GetType(), type);
    EXPECT_EQ(uniform.GetHide(), hide);
  }

  void TestIntSerialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = 1;
    UniformType type = UniformType::Int;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    SerializeUniformTestWrapper(uniform);
  }

  void TestFloatSerialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = 1.0f;
    UniformType type = UniformType::Float;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    SerializeUniformTestWrapper(uniform);
  }

  void TestVec2Serialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = glm::vec2(1.0f, 2.0f);
    UniformType type = UniformType::Vec2;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    SerializeUniformTestWrapper(uniform);
  }

  void TestVec3Serialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = glm::vec3(1.0f, 2.0f, 3.0f);
    UniformType type = UniformType::Vec3;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    SerializeUniformTestWrapper(uniform);
  }

  void TestVec4Serialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
    UniformType type = UniformType::Vec4;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    SerializeUniformTestWrapper(uniform);
  }

  void TestMat4Serialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = glm::mat4(1.0f);
    UniformType type = UniformType::Mat4;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

    SerializeUniformTestWrapper(uniform);
  }
};

TEST_F(ShaderUniformTest, TestUniform) { TestUniform(); }
TEST_F(ShaderUniformTest, TestIntSerialization) {
  TestIntSerialization();
}
TEST_F(ShaderUniformTest, TestFloatSerialization) {
  TestFloatSerialization();
}
TEST_F(ShaderUniformTest, TestVec2Serialization) {
  TestVec2Serialization();
}
TEST_F(ShaderUniformTest, TestVec3Serialization) {
  TestVec3Serialization();
}
TEST_F(ShaderUniformTest, TestVec4Serialization) {
  TestVec4Serialization();
}
TEST_F(ShaderUniformTest, TestMat4Serialization) {
  TestMat4Serialization();
}