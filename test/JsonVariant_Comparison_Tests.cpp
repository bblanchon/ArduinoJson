// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonVariant_Comparison_Tests : public ::testing::Test {
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
    actual = expected;
  }

  template <typename T>
  void mustBeEqualTo(T expected) {
    EXPECT_EQ(expected, actual);  // operator==
    EXPECT_EQ(actual, expected);  // operator==
    EXPECT_LE(expected, actual);  // operator<=
    EXPECT_LE(actual, expected);  // operator<=
    EXPECT_GE(expected, actual);  // operator>=
    EXPECT_GE(actual, expected);  // operator>=
  }

  template <typename T>
  void mustBeGreaterThan(T expected) {
    EXPECT_GT(actual, expected);  // operator>
    EXPECT_LT(expected, actual);  // operator<
    EXPECT_NE(actual, expected);  // operator!=
    EXPECT_NE(expected, actual);  // operator!=
  }

  template <typename T>
  void mustBeLessThan(T expected) {
    EXPECT_LT(actual, expected);  // operator<
    EXPECT_GT(expected, actual);  // operator<
    EXPECT_NE(actual, expected);  // operator!=
    EXPECT_NE(expected, actual);  // operator!=
  }

  JsonVariant actual;
};

TEST_F(JsonVariant_Comparison_Tests, Double) {
  testValue<double>(123.44, 123.45, 123.46);
}

TEST_F(JsonVariant_Comparison_Tests, Float) {
  testValue<float>(123.44f, 123.45f, 123.46f);
}

TEST_F(JsonVariant_Comparison_Tests, SChar) {
  testValue<signed char>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, SInt) {
  testValue<signed int>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, SLong) {
  testValue<signed long>(122L, 123L, 124L);
}

TEST_F(JsonVariant_Comparison_Tests, SShort) {
  testValue<signed short>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, UChar) {
  testValue<unsigned char>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, UInt) {
  testValue<unsigned int>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, ULong) {
  testValue<unsigned long>(122L, 123L, 124L);
}

TEST_F(JsonVariant_Comparison_Tests, UShort) {
  testValue<unsigned short>(122, 123, 124);
}
