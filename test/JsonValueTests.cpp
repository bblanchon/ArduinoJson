// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonValue.hpp>

using namespace ArduinoJson;

class JsonValueTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    jsonValue1 = json.createValue();
    jsonValue2 = json.createValue();
  }

  StaticJsonBuffer<42> json;
  JsonValue jsonValue1;
  JsonValue jsonValue2;
};

TEST_F(JsonValueTests, CanStoreInteger) {
  jsonValue1 = 123;

  EXPECT_EQ(123, jsonValue1.as<int>());
}

TEST_F(JsonValueTests, CanStoreDouble) {
  jsonValue1 = 123.45;

  EXPECT_EQ(123.45, jsonValue1.as<double>());
}

TEST_F(JsonValueTests, CanStoreTrue) {
  jsonValue1 = true;
  EXPECT_TRUE(jsonValue1.as<bool>());
}

TEST_F(JsonValueTests, CanStoreFalse) {
  jsonValue1 = false;
  EXPECT_FALSE(jsonValue1.as<bool>());
}

TEST_F(JsonValueTests, CanStoreString) {
  jsonValue1 = "hello";

  EXPECT_STREQ("hello", jsonValue1.as<const char *>());
}

TEST_F(JsonValueTests, CanStoreObject) {
  JsonObject innerObject1 = json.createObject();

  jsonValue1 = innerObject1;

  EXPECT_EQ(innerObject1, jsonValue1.as<JsonObject>());
}

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
  JsonObject object = json.createObject();

  jsonValue1 = object;

  object["hello"] = "world";

  EXPECT_EQ(1, jsonValue1.as<JsonObject>().size());
}

TEST_F(JsonValueTests, ArraysAreCopiedByReference) {
  JsonArray array = json.createArray();

  jsonValue1 = array;

  array.add("world");

  EXPECT_EQ(1, jsonValue1.as<JsonArray>().size());
}
