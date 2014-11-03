// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonArray.hpp>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/JsonValue.hpp>

using namespace ArduinoJson;

class JsonValueTests : public ::testing::Test {
 protected:
  StaticJsonBuffer<200> json;
  JsonValue jsonValue1;
  JsonValue jsonValue2;
};

TEST_F(JsonValueTests, IntegersAreCopiedByValue) {
  jsonValue1 = 123;
  jsonValue2 = jsonValue1;
  jsonValue1 = 456;

  EXPECT_EQ(123, jsonValue2.as<int>());
}

TEST_F(JsonValueTests, DoublesAreCopiedByValue) {
  jsonValue1 = 123.45;
  jsonValue2 = jsonValue1;
  jsonValue1 = 456.78;

  EXPECT_EQ(123.45, jsonValue2.as<double>());
}

TEST_F(JsonValueTests, BooleansAreCopiedByValue) {
  jsonValue1 = true;
  jsonValue2 = jsonValue1;
  jsonValue1 = false;

  EXPECT_TRUE(jsonValue2.as<bool>());
}

TEST_F(JsonValueTests, StringsAreCopiedByValue) {
  jsonValue1 = "hello";
  jsonValue2 = jsonValue1;
  jsonValue1 = "world";

  EXPECT_STREQ("hello", jsonValue2.as<const char *>());
}

TEST_F(JsonValueTests, ObjectsAreCopiedByReference) {
  JsonObject &object = json.createObject();

  jsonValue1 = object;

  object["hello"] = "world";

  EXPECT_EQ(1, jsonValue1.asObject().size());
}

TEST_F(JsonValueTests, ArraysAreCopiedByReference) {
  JsonArray &array = json.createArray();

  jsonValue1 = array;

  array.add("world");

  EXPECT_EQ(1, jsonValue1.asArray().size());
}
