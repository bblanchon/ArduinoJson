// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonValue.hpp>

using namespace ArduinoJson;

TEST(StaticJsonBuffer, CapacityMatchTemplateParameter) {
  StaticJsonBuffer<42> json;
  EXPECT_EQ(42, json.capacity());
}

TEST(StaticJsonBuffer, InitialSizeIsZero) {
  StaticJsonBuffer<42> json;
  EXPECT_EQ(0, json.size());
}

TEST(StaticJsonBuffer, WhenCreateObjectIsCalled_ThenSizeIsIncreasedByOne) {
  StaticJsonBuffer<42> json;

  json.createObject();
  EXPECT_EQ(1, json.size());

  json.createObject();
  EXPECT_EQ(2, json.size());
}

TEST(StaticJsonBuffer,
     GivenBufferIsFull_WhenCreateObjectIsCalled_ThenSizeDoesNotChange) {
  StaticJsonBuffer<1> json;

  json.createObject();
  EXPECT_EQ(1, json.size());

  json.createObject();
  EXPECT_EQ(1, json.size());
}

TEST(StaticJsonBuffer,
     WhenCreateObjectIsCalled_ThenAnEmptyJsonObjectIsReturned) {
  StaticJsonBuffer<42> json;

  JsonObject obj = json.createObject();
  EXPECT_EQ(0, obj.size());
}

TEST(StaticJsonBuffer,
     GivenAJsonObject_WhenValuesAreAdded_ThenSizeIsIncreasedByTwo) {
  StaticJsonBuffer<42> json;
  JsonObject obj = json.createObject();

  obj["hello"];
  EXPECT_EQ(3, json.size());

  obj["world"];
  EXPECT_EQ(5, json.size());
}

TEST(
    StaticJsonBuffer,
    GivenAJsonObject_WhenSameValuesAreAddedTwice_ThenSizeIsOnlyIncreasedByTwo) {
  StaticJsonBuffer<42> json;
  JsonObject obj = json.createObject();

  obj["hello"];
  EXPECT_EQ(3, json.size());

  obj["hello"];
  EXPECT_EQ(3, json.size());
}