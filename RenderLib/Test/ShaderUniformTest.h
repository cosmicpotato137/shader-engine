#pragma once

#include "gtest/gtest.h"
#include "ShaderUniform.h"

class ShaderUniformTest : public ::testing::Test {
protected:
  virtual void SetUp() {}

  virtual void TearDown() {}

  void TestSerialization() {
    std::stringstream ss;
    Uniform uniform =
        Uniform("test", 0, glm::vec3(1, 0, 0), UniformType::Vec3, false);
    ss << uniform;
    Uniform deserialized;
    ss >> deserialized;
    EXPECT_EQ(uniform.GetName(), deserialized.GetName());
    EXPECT_EQ(uniform.GetType(), deserialized.GetType());
    EXPECT_EQ(uniform.GetHide(), deserialized.GetHide());
    EXPECT_EQ(uniform.GetValue(), deserialized.GetValue());
  }
};

TEST_F(ShaderUniformTest, Serialization) { TestSerialization(); }