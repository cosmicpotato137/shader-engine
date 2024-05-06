#pragma once
#include "gtest/gtest.h"
#include "Core/Serial.h"

#include <filesystem>

class TestSerializable {
public:
  TestSerializable() : a(0), b(0) {}
  TestSerializable(int a, int b) : a(a), b(b) {}
  int a, b;

private:
  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & a;
    ar & b;
  }
};

SE_SERIAL_VERSION(TestSerializable, 1)

class SerializationTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}

  void TestSave() {
    TestSerializable testSerializable(1, 2);
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "debug" / "data" / "TestSerializable.dat";
    Serial::Save(testSerializable, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));
  }

  void TestLoad() {
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "debug" / "data" / "TestSerializable.dat";

    uptr<TestSerializable> testSerializablePtr =
        Serial::Load<TestSerializable>(output_path.string());

    // Check if object is not null
    ASSERT_NE(testSerializablePtr, nullptr);

    // Check if object has correct values
    ASSERT_EQ(testSerializablePtr->a, 1);
    ASSERT_EQ(testSerializablePtr->b, 2);
  }
};

TEST_F(SerializationTest, TestSave) { TestSave(); }
TEST_F(SerializationTest, TestLoad) { TestLoad(); }