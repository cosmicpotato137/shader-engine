#pragma once
#include "gtest/gtest.h"
#include "Scene/Serializable.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <filesystem>

class TestSerializable : public Serializable {
public:
  TestSerializable() : Serializable() {}
  TestSerializable(int a, int b) : Serializable(), a(a), b(b) {}

  void
  serialize(boost::archive::text_oarchive &ar, const unsigned int version) {
    ar & a;
    ar & b;
  }

  void
  serialize(boost::archive::text_iarchive &ar, const unsigned int version) {
    ar & a;
    ar & b;
  }

  int a = 0;
  int b = 0;
};

class SerializationTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}

  void TestSave() {
    TestSerializable testSerializable(1, 2);
    auto current_path = std::filesystem::current_path();
    auto output_path = current_path / "data" / "TestSerializable.dat";
    testSerializable.Save(output_path.string());

    // Check if file exists
    ASSERT_TRUE(std::filesystem::exists(output_path));
  }

  void TestLoad() {
    auto current_path = std::filesystem::current_path();
    auto output_path = current_path / "data" / "TestSerializable.dat";
    TestSerializable *testSerializable = dynamic_cast<TestSerializable *>(
        TestSerializable::Load(output_path.string()));

    // Check if object is not null
    ASSERT_TRUE(testSerializable != nullptr);

    // Check if object has correct values
    ASSERT_EQ(testSerializable->a, 1);
    ASSERT_EQ(testSerializable->b, 2);
  }
};

TEST_F(SerializationTest, TestSave) { TestSave(); }
TEST_F(SerializationTest, TestLoad) { TestLoad(); }