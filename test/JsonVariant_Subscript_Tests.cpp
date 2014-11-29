// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonVariant_Subscript_Tests : public ::testing::Test {
 protected:
  StaticJsonBuffer<200> buffer;
  JsonVariant variant;
};

TEST_F(JsonVariant_Subscript_Tests, Array) {
  JsonArray &array = buffer.createArray();
  array.add("element at index 0");
  array.add("element at index 1");

  variant = array;

  EXPECT_EQ(2, variant.size());
  EXPECT_STREQ("element at index 0", variant[0].asString());
  EXPECT_STREQ("element at index 1", variant[1].asString());
  EXPECT_FALSE(variant[-1].success());
  EXPECT_FALSE(variant[3].success());
  EXPECT_FALSE(variant["0"].success());
}

TEST_F(JsonVariant_Subscript_Tests, Object) {
  JsonObject &object = buffer.createObject();
  object["a"] = "element at key \"a\"";
  object["b"] = "element at key \"b\"";

  variant = object;

  EXPECT_EQ(2, variant.size());
  EXPECT_STREQ("element at key \"a\"", variant["a"].asString());
  EXPECT_STREQ("element at key \"b\"", variant["b"].asString());
  EXPECT_FALSE(variant["c"].success());
  EXPECT_FALSE(variant[0].success());
}

TEST_F(JsonVariant_Subscript_Tests, Undefined) {
  variant = JsonVariant();
  EXPECT_EQ(0, variant.size());
  EXPECT_FALSE(variant["0"].success());
  EXPECT_FALSE(variant[0].success());
}

TEST_F(JsonVariant_Subscript_Tests, Invalid) {
  variant = JsonVariant::invalid();
  EXPECT_EQ(0, variant.size());
  EXPECT_FALSE(variant["0"].success());
  EXPECT_FALSE(variant[0].success());
}

TEST_F(JsonVariant_Subscript_Tests, String) {
  variant = "hello world";
  EXPECT_EQ(0, variant.size());
  EXPECT_FALSE(variant["0"].success());
  EXPECT_FALSE(variant[0].success());
}
