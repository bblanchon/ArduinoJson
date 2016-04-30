// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonParser_Variant_Test : public testing::Test {
 protected:
  void whenInputIs(const char* jsonString) {
    strcpy(_jsonString, jsonString);
    _result = _jsonBuffer.parse(_jsonString);
  }

  template <typename T>
  void resultMustEqual(T expected) {
    EXPECT_EQ(expected, _result.as<T>());
  }

  void resultMustEqual(const char* expected) {
    EXPECT_STREQ(expected, _result.as<char*>());
  }

  template <typename T>
  void resultTypeMustBe() {
    EXPECT_TRUE(_result.is<T>());
  }

 private:
  DynamicJsonBuffer _jsonBuffer;
  JsonVariant _result;
  char _jsonString[256];
};

TEST_F(JsonParser_Variant_Test, EmptyObject) {
  whenInputIs("{}");
  resultTypeMustBe<JsonObject>();
}

TEST_F(JsonParser_Variant_Test, EmptyArray) {
  whenInputIs("[]");
  resultTypeMustBe<JsonArray>();
}

TEST_F(JsonParser_Variant_Test, Integer) {
  whenInputIs("42");
  resultTypeMustBe<int>();
  resultMustEqual(42);
}

TEST_F(JsonParser_Variant_Test, Double) {
  whenInputIs("3.14");
  resultTypeMustBe<double>();
  resultMustEqual(3.14);
}

TEST_F(JsonParser_Variant_Test, String) {
  whenInputIs("\"hello world\"");
  resultTypeMustBe<char*>();
  resultMustEqual("hello world");
}

TEST_F(JsonParser_Variant_Test, True) {
  whenInputIs("true");
  resultTypeMustBe<bool>();
  resultMustEqual(true);
}

TEST_F(JsonParser_Variant_Test, False) {
  whenInputIs("false");
  resultTypeMustBe<bool>();
  resultMustEqual(false);
}
