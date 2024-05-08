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

  void TestLoadNew() {
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "debug" / "data" / "TestSerializable.dat";

    TestSerializable testSerializable =
        Serial::Load<TestSerializable>(output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializable.a, 1);
    ASSERT_EQ(testSerializable.b, 2);
  }

  void TestLoadInplace() {
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "debug" / "data" / "TestSerializable.dat";

    TestSerializable testSerializable;
    Serial::LoadInplace(testSerializable, output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializable.a, 1);
    ASSERT_EQ(testSerializable.b, 2);
  }

  void TestSharedPtrSave() {
    std::shared_ptr<TestSerializable> testSerializable =
        std::make_shared<TestSerializable>(1, 2);
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";
    Serial::Save(testSerializable, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));
  }

  void TestSharedPtrLoadNew() {
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";

    std::shared_ptr<TestSerializable> testSerializablePtr =
        Serial::Load<std::shared_ptr<TestSerializable>>(output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializablePtr->a, 1);
    ASSERT_EQ(testSerializablePtr->b, 2);
  }

  void TestSharedPtrLoadInplace() {
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";

    std::shared_ptr<TestSerializable> testSerializablePtr;
    Serial::LoadInplace(testSerializablePtr, output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializablePtr->a, 1);
    ASSERT_EQ(testSerializablePtr->b, 2);
  }

  void TestMap() {
    std::map<std::string, TestSerializable> testMap;
    testMap["test1"] = TestSerializable(1, 2);
    testMap["test2"] = TestSerializable(3, 4);

    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";
    Serial::Save(testMap, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));

    std::map<std::string, TestSerializable> testMapLoaded =
        Serial::Load<std::map<std::string, TestSerializable>>(
            output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testMapLoaded["test1"].a, 1);
    ASSERT_EQ(testMapLoaded["test1"].b, 2);
    ASSERT_EQ(testMapLoaded["test2"].a, 3);
    ASSERT_EQ(testMapLoaded["test2"].b, 4);
  }

  void TestPtrMap() {
    std::map<std::string, std::shared_ptr<TestSerializable>> testMap;
    testMap["test1"] = std::make_shared<TestSerializable>(1, 2);
    testMap["test2"] = std::make_shared<TestSerializable>(3, 4);

    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";
    Serial::Save(testMap, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));

    std::map<std::string, std::shared_ptr<TestSerializable>> testMapLoaded =
        Serial::Load<std::map<std::string, std::shared_ptr<TestSerializable>>>(
            output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testMapLoaded["test1"]->a, 1);
    ASSERT_EQ(testMapLoaded["test1"]->b, 2);
    ASSERT_EQ(testMapLoaded["test2"]->a, 3);
    ASSERT_EQ(testMapLoaded["test2"]->b, 4);
  }
};

TEST_F(SerializationTest, TestSave) { TestSave(); }
TEST_F(SerializationTest, TestLoadNew) { TestLoadNew(); }
TEST_F(SerializationTest, TestLoadInplace) { TestLoadInplace(); }
TEST_F(SerializationTest, TestSharedPtrSave) { TestSharedPtrSave(); }
TEST_F(SerializationTest, TestSharedPtrLoadNew) { TestSharedPtrLoadNew(); }
TEST_F(SerializationTest, TestSharedPtrLoadInplace) {
  TestSharedPtrLoadInplace();
}
TEST_F(SerializationTest, TestMap) { TestMap(); }
TEST_F(SerializationTest, TestPtrMap) { TestPtrMap(); }