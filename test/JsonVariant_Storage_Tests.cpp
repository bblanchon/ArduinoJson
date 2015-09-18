// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonVariant_Storage_Tests : public ::testing::Test {
 protected:
  template <typename T>
  void testValue(T expected) {
    _actual = expected;
    EXPECT_EQ(expected, _actual.as<T>());
  }

  template <typename T>
  void testReference(T &expected) {
    _actual = expected;
    EXPECT_EQ(expected, _actual.as<T &>());
  }

 private:
  JsonVariant _actual;
};

TEST_F(JsonVariant_Storage_Tests, Double) { testValue<double>(123.45); }
TEST_F(JsonVariant_Storage_Tests, False) { testValue<bool>(false); }
TEST_F(JsonVariant_Storage_Tests, Float) { testValue<float>(123.45f); }
TEST_F(JsonVariant_Storage_Tests, Null) { testValue<const char *>(NULL); }
TEST_F(JsonVariant_Storage_Tests, SChar) { testValue<signed char>(123); }
TEST_F(JsonVariant_Storage_Tests, SInt) { testValue<signed int>(123); }
TEST_F(JsonVariant_Storage_Tests, SLong) { testValue<signed long>(123L); }
TEST_F(JsonVariant_Storage_Tests, SShort) { testValue<signed short>(123); }
TEST_F(JsonVariant_Storage_Tests, String) { testValue<const char *>("hello"); }
TEST_F(JsonVariant_Storage_Tests, True) { testValue<bool>(true); }
TEST_F(JsonVariant_Storage_Tests, UChar) { testValue<unsigned char>(123); }
TEST_F(JsonVariant_Storage_Tests, UInt) { testValue<unsigned int>(123U); }
TEST_F(JsonVariant_Storage_Tests, ULong) { testValue<unsigned long>(123UL); }
TEST_F(JsonVariant_Storage_Tests, UShort) { testValue<unsigned short>(123); }

TEST_F(JsonVariant_Storage_Tests, CanStoreObject) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &object = jsonBuffer.createObject();

  testReference(object);
}
