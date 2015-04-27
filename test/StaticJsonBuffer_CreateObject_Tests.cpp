// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(StaticJsonBuffer_CreateObject_Tests, GrowsWithObject) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(3)> json;

  JsonObject &obj = json.createObject();
  ASSERT_EQ(JSON_OBJECT_SIZE(0), json.size());

  obj["hello"];
  ASSERT_EQ(JSON_OBJECT_SIZE(1), json.size());

  obj["world"];
  ASSERT_EQ(JSON_OBJECT_SIZE(2), json.size());

  obj["world"];  // <- same value, should not grow
  ASSERT_EQ(JSON_OBJECT_SIZE(2), json.size());
}

TEST(StaticJsonBuffer_CreateObject_Tests, SucceedWhenBigEnough) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(0)> json;

  JsonObject &object = json.createObject();
  ASSERT_TRUE(object.success());
}

TEST(StaticJsonBuffer_CreateObject_Tests, FailsWhenTooSmall) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(0) - 1> json;

  JsonObject &object = json.createObject();
  ASSERT_FALSE(object.success());
}

TEST(StaticJsonBuffer_CreateObject_Tests, ObjectDoesntGrowWhenFull) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1)> json;

  JsonObject &obj = json.createObject();
  obj["hello"];
  obj["world"];

  ASSERT_EQ(JSON_OBJECT_SIZE(1), json.size());
}
