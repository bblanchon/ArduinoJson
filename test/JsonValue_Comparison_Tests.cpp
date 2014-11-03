// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/JsonValue.hpp>
#include "Printers.hpp"

using namespace ArduinoJson;

class JsonValue_Comparison_Tests : public ::testing::Test {
 protected:
  template <typename T>
  void testValue(T low, T mid, T high) {
    setValueTo(mid);
    mustBeEqualTo(mid);
    mustBeGreaterThan(low);
    mustBeLessThan(high);
  }

 private:
  template <typename T>
  void setValueTo(T expected) {
    jsonValue = expected;
  }

  template <typename T>
  void mustBeEqualTo(T expected) {
    EXPECT_EQ(expected, jsonValue);  // operator==
    EXPECT_EQ(jsonValue, expected);  // operator==
    EXPECT_LE(expected, jsonValue);  // operator<=
    EXPECT_LE(jsonValue, expected);  // operator<=
    EXPECT_GE(expected, jsonValue);  // operator>=
    EXPECT_GE(jsonValue, expected);  // operator>=
  }

  template <typename T>
  void mustBeGreaterThan(T expected) {
    EXPECT_GT(jsonValue, expected);  // operator>
    EXPECT_LT(expected, jsonValue);  // operator<
    EXPECT_NE(jsonValue, expected);  // operator!=
    EXPECT_NE(expected, jsonValue);  // operator!=
  }

  template <typename T>
  void mustBeLessThan(T expected) {
    EXPECT_LT(jsonValue, expected);  // operator<
    EXPECT_GT(expected, jsonValue);  // operator<
    EXPECT_NE(jsonValue, expected);  // operator!=
    EXPECT_NE(expected, jsonValue);  // operator!=
  }

  JsonValue jsonValue;
};

TEST_F(JsonValue_Comparison_Tests, Double) {
  testValue<double>(123.44, 123.45, 123.46);
}

TEST_F(JsonValue_Comparison_Tests, Float) {
  testValue<float>(123.44f, 123.45f, 123.46f);
}

TEST_F(JsonValue_Comparison_Tests, SChar) {
  testValue<signed char>(122, 123, 124);
}

TEST_F(JsonValue_Comparison_Tests, SInt) {
  testValue<signed int>(122, 123, 124);
}

TEST_F(JsonValue_Comparison_Tests, SLong) {
  testValue<signed long>(122L, 123L, 124L);
}

TEST_F(JsonValue_Comparison_Tests, SShort) {
  testValue<signed short>(122, 123, 124);
}

TEST_F(JsonValue_Comparison_Tests, UChar) {
  testValue<unsigned char>(122, 123, 124);
}

TEST_F(JsonValue_Comparison_Tests, UInt) {
  testValue<unsigned int>(122, 123, 124);
}

TEST_F(JsonValue_Comparison_Tests, ULong) {
  testValue<unsigned long>(122L, 123L, 124L);
}

TEST_F(JsonValue_Comparison_Tests, UShort) {
  testValue<unsigned short>(122, 123, 124);
}
