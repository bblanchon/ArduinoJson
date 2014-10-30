// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/JsonValue.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

TEST(StaticJsonBuffer, CapacityMatchTemplateParameter) {
  StaticJsonBuffer<42> json;
  EXPECT_EQ(42, json.capacity());
}

TEST(StaticJsonBuffer, InitialSizeIsZero) {
  StaticJsonBuffer<42> json;
  EXPECT_EQ(0, json.size());
}

TEST(StaticJsonBuffer, WhenCreateObjectIsCalled_ThenSizeIsIncreasedSizeOfJsonObject) {
  StaticJsonBuffer<42> json;

  json.createObject();
  EXPECT_EQ(sizeof(JsonObject), json.size());

  json.createObject();
  EXPECT_EQ(sizeof(JsonObject)*2, json.size());
}

TEST(StaticJsonBuffer,
     GivenBufferIsFull_WhenCreateObjectIsCalled_ThenSizeDoesNotChange) {
  StaticJsonBuffer<sizeof(JsonObject)> json;

  json.createObject();
  EXPECT_EQ(sizeof(JsonObject), json.size());

  json.createObject();
  EXPECT_EQ(sizeof(JsonObject), json.size());
}

TEST(StaticJsonBuffer,
     WhenCreateObjectIsCalled_ThenAnEmptyJsonObjectIsReturned) {
  StaticJsonBuffer<42> json;

  JsonObject &obj = json.createObject();
  EXPECT_EQ(0, obj.size());
}

TEST(StaticJsonBuffer,
     GivenAJsonObject_WhenValuesAreAdded_ThenSizeIsIncreasedAccordingly) {
  StaticJsonBuffer<200> json;
  JsonObject &obj = json.createObject();

  obj["hello"];
  EXPECT_EQ(sizeof(JsonObject)+sizeof(Internals::JsonObjectNode), json.size());

  obj["world"];
  EXPECT_EQ(sizeof(JsonObject) + sizeof(Internals::JsonObjectNode)*2, json.size());
}

TEST(
    StaticJsonBuffer,
    GivenAJsonObject_WhenSameValuesAreAddedTwice_ThenSizeIsOnlyIncreasedByTwo) {
  StaticJsonBuffer<42> json;
  JsonObject &obj = json.createObject();

  obj["hello"];

  size_t sizeBefore = json.size();
  obj["hello"];
  size_t sizeAfter = json.size();

  EXPECT_EQ(sizeBefore, sizeAfter);
}
