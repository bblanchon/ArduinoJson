// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonVariant_Subscript_Tests : public ::testing::Test {
 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonVariant _variant;
};

TEST_F(JsonVariant_Subscript_Tests, Array) {
  JsonArray &array = _jsonBuffer.createArray();
  array.add("element at index 0");
  array.add("element at index 1");

  _variant = array;

  EXPECT_EQ(2, _variant.size());
  EXPECT_STREQ("element at index 0", _variant[0].asString());
  EXPECT_STREQ("element at index 1", _variant[1].asString());
  EXPECT_FALSE(_variant[-1].success());
  EXPECT_FALSE(_variant[3].success());
  EXPECT_FALSE(_variant["0"].success());
}

TEST_F(JsonVariant_Subscript_Tests, Object) {
  JsonObject &object = _jsonBuffer.createObject();
  object["a"] = "element at key \"a\"";
  object["b"] = "element at key \"b\"";

  _variant = object;

  EXPECT_EQ(2, _variant.size());
  EXPECT_STREQ("element at key \"a\"", _variant["a"].asString());
  EXPECT_STREQ("element at key \"b\"", _variant["b"].asString());
  EXPECT_FALSE(_variant["c"].success());
  EXPECT_FALSE(_variant[0].success());
}

TEST_F(JsonVariant_Subscript_Tests, Undefined) {
  _variant = JsonVariant();
  EXPECT_EQ(0, _variant.size());
  EXPECT_FALSE(_variant["0"].success());
  EXPECT_FALSE(_variant[0].success());
}

TEST_F(JsonVariant_Subscript_Tests, String) {
  _variant = "hello world";
  EXPECT_EQ(0, _variant.size());
  EXPECT_FALSE(_variant["0"].success());
  EXPECT_FALSE(_variant[0].success());
}
