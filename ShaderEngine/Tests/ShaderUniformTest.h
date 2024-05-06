#pragma once

#include "gtest/gtest.h"
#include "Renderer/ShaderUniform.h"

#include <filesystem>

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

  void TestUniformSerialization() {
    std::string name = "test";
    int location = 0;
    uniform_types value = 1;
    UniformType type = UniformType::Int;
    bool hide = false;
    Uniform uniform(name, location, value, type, hide);

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
};

TEST_F(ShaderUniformTest, TestUniform) { TestUniform(); }
TEST_F(ShaderUniformTest, TestUniformSerialization) {
  TestUniformSerialization();
}