// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(JsonObject_Invalid_Tests, AtFails) {
  ASSERT_FALSE(JsonObject::invalid().at(0).success());
}

TEST(JsonObject_Invalid_Tests, SubscriptFails) {
  ASSERT_FALSE(JsonObject::invalid()[0].success());
}

TEST(JsonObject_Invalid_Tests, AddFails) {
  JsonObject& array = JsonObject::invalid();
  array.add("hello", "world");
  ASSERT_EQ(0, array.size());
}

TEST(JsonObject_Invalid_Tests, CreateNestedArrayFails) {
  ASSERT_FALSE(JsonObject::invalid().createNestedArray("hello").success());
}

TEST(JsonObject_Invalid_Tests, CreateNestedObjectFails) {
  ASSERT_FALSE(JsonObject::invalid().createNestedObject("world").success());
}

TEST(JsonObject_Invalid_Tests, PrintToWritesBraces) {
  char buffer[32];
  JsonObject::invalid().printTo(buffer, sizeof(buffer));
  ASSERT_STREQ("{}", buffer);
}