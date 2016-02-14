// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonVariant_PrintTo_Tests : public testing::Test {
 protected:
  JsonVariant variant;

  void outputMustBe(const char *expected) {
    char buffer[256] = "";
    size_t n = variant.printTo(buffer, sizeof(buffer));
    EXPECT_STREQ(expected, buffer);
    EXPECT_EQ(strlen(expected), n);
  }
};

TEST_F(JsonVariant_PrintTo_Tests, Empty) { outputMustBe(""); }

TEST_F(JsonVariant_PrintTo_Tests, Null) {
  variant = static_cast<char *>(0);
  outputMustBe("null");
}

TEST_F(JsonVariant_PrintTo_Tests, String) {
  variant = "hello";
  outputMustBe("\"hello\"");
}

TEST_F(JsonVariant_PrintTo_Tests, DoubleZero) {
  variant = 0.0;
  outputMustBe("0.00");
}

TEST_F(JsonVariant_PrintTo_Tests, DoubleDefaultDigits) {
  variant = 3.14159265358979323846;
  outputMustBe("3.14");
}

TEST_F(JsonVariant_PrintTo_Tests, DoubleFourDigits) {
  variant = JsonVariant(3.14159265358979323846, 4);
  outputMustBe("3.1416");
}

TEST_F(JsonVariant_PrintTo_Tests, Integer) {
  variant = 42;
  outputMustBe("42");
}

TEST_F(JsonVariant_PrintTo_Tests, Long) {
  variant = 42L;
  outputMustBe("42");
}

TEST_F(JsonVariant_PrintTo_Tests, Char) {
  variant = '*';
  outputMustBe("42");
}

TEST_F(JsonVariant_PrintTo_Tests, True) {
  variant = true;
  outputMustBe("true");
}

TEST_F(JsonVariant_PrintTo_Tests, OneFalse) {
  variant = false;
  outputMustBe("false");
}

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
TEST_F(JsonVariant_PrintTo_Tests, NegativeInt64) {
  variant = -9223372036854775807 - 1;
  outputMustBe("-9223372036854775808");
}

TEST_F(JsonVariant_PrintTo_Tests, PositiveInt64) {
  variant = 9223372036854775807;
  outputMustBe("9223372036854775807");
}
#endif
