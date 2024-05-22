#pragma once
#include "gtest/gtest.h"
#include "Core/Serial.h"

#include <filesystem>

class TestSerializable {
public:
  TestSerializable() : a(0), b(0) {}
  TestSerializable(int a, int b) : a(a), b(b) {}
  int a, b;

  virtual ~TestSerializable() {}

private:
  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & a;
    ar & b;
  }
};

BOOST_CLASS_EXPORT(TestSerializable)

class TestSerializableChild : public TestSerializable {
public:
  TestSerializableChild() : TestSerializable(), c(0) {}
  TestSerializableChild(int a, int b, int c) : TestSerializable(a, b), c(c) {}
  int c;

private:
  SE_SERIAL_FRIENDS;
  template <class Archive> void serialize(Archive &ar, const unsigned int) {
    ar &boost::serialization::base_object<TestSerializable>(*this);
    ar & c;
  }
};

BOOST_CLASS_EXPORT(TestSerializableChild)

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

    TestSerializable testSerializable =
        Serial::Load<TestSerializable>(output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializable.a, 1);
    ASSERT_EQ(testSerializable.b, 2);
  }

  void TestSharedPtrLoad() {
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";

    std::shared_ptr<TestSerializable> testSerializable = std::make_shared<TestSerializable>(1, 2);
    Serial::Save(testSerializable, output_path.string());

    std::shared_ptr<TestSerializable> testSerializablePtr =
        Serial::Load<std::shared_ptr<TestSerializable>>(output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializablePtr->a, 1);
    ASSERT_EQ(testSerializablePtr->b, 2);
  }

  void TestBaseClass() {
    TestSerializableChild testSerializable(1, 2, 3);
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";
    Serial::Save(testSerializable, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));

    TestSerializableChild testSerializableLoaded =
        Serial::Load<TestSerializableChild>(output_path.string());

    // Check if object has correct values
    ASSERT_EQ(testSerializableLoaded.a, 1);
    ASSERT_EQ(testSerializableLoaded.b, 2);
    ASSERT_EQ(testSerializableLoaded.c, 3);
  }

  void TestBaseClassPtr() {
    TestSerializable *testSerializable = new TestSerializableChild(1, 2, 3);
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";

    Serial::Save(testSerializable, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));

    TestSerializable *testSerializablePtr = Serial::Load<TestSerializable *>(
        output_path.string());
    TestSerializableChild *testSerializableChildPtr =
        dynamic_cast<TestSerializableChild *>(testSerializablePtr);

    // Check if object has correct values
    ASSERT_EQ(testSerializableChildPtr->a, 1);
    ASSERT_EQ(testSerializableChildPtr->b, 2);
    ASSERT_EQ(testSerializableChildPtr->c, 3);

    delete testSerializable;
    delete testSerializablePtr;
  }

  void TestBaseClassSharedPtr() {
    std::shared_ptr<TestSerializable> testSerializable =
        std::make_shared<TestSerializableChild>(1, 2, 3);
    auto current_path = std::filesystem::current_path();
    auto output_path =
        current_path / "bin" / "Debug" / "data" / "TestSerializable.dat";
    Serial::Save(testSerializable, output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));

    std::shared_ptr<TestSerializable> testSerializablePtr =
        Serial::Load<std::shared_ptr<TestSerializable>>(output_path.string());

    std::shared_ptr<TestSerializableChild> testSerializableChildPtr =
        std::dynamic_pointer_cast<TestSerializableChild>(testSerializablePtr);

    // Check if object has correct values
    ASSERT_EQ(testSerializableChildPtr->a, 1);
    ASSERT_EQ(testSerializableChildPtr->b, 2);
    ASSERT_EQ(testSerializableChildPtr->c, 3);
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
TEST_F(SerializationTest, TestLoad) { TestLoad(); }
TEST_F(SerializationTest, TestBaseClassPtr) { TestBaseClassPtr(); }
TEST_F(SerializationTest, TestSharedPtrLoad) { TestSharedPtrLoad(); }
TEST_F(SerializationTest, TestBaseClass) { TestBaseClass(); }
TEST_F(SerializationTest, TestBaseClassSharedPtr) { TestBaseClassSharedPtr(); }
TEST_F(SerializationTest, TestMap) { TestMap(); }
TEST_F(SerializationTest, TestPtrMap) { TestPtrMap(); }