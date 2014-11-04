// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/JsonVariant.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

const size_t SIZEOF_ONE_OBJECT = JSON_OBJECT_SIZE(0);
const size_t SIZEOF_TWO_OBJECTS = 2 * SIZEOF_ONE_OBJECT;
const size_t SIZEOF_OBJECT_WITH_ONE_VALUE = JSON_OBJECT_SIZE(1);
const size_t SIZEOF_OBJECT_WITH_TWO_VALUES = JSON_OBJECT_SIZE(2);

TEST(StaticJsonBuffer, CapacityMatchTemplateParameter) {
  StaticJsonBuffer<42> json;

  EXPECT_EQ(42, json.capacity());
}

TEST(StaticJsonBuffer, InitialSizeIsZero) {
  StaticJsonBuffer<42> json;

  EXPECT_EQ(0, json.size());
}

TEST(StaticJsonBuffer,
     WhenCreateObjectIsCalled_ThenSizeIsIncreasedSizeOfJsonObject) {
  StaticJsonBuffer<SIZEOF_TWO_OBJECTS> json;

  json.createObject();
  json.createObject();
  EXPECT_EQ(SIZEOF_TWO_OBJECTS, json.size());
}

TEST(StaticJsonBuffer,
     GivenBufferIsFull_WhenCreateObjectIsCalled_ThenSizeDoesNotChange) {
  StaticJsonBuffer<SIZEOF_ONE_OBJECT> json;

  json.createObject();
  json.createObject();
  EXPECT_EQ(SIZEOF_ONE_OBJECT, json.size());
}

TEST(StaticJsonBuffer,
     WhenCreateObjectIsCalled_ThenAnEmptyJsonObjectIsReturned) {
  StaticJsonBuffer<SIZEOF_ONE_OBJECT> json;

  JsonObject &obj = json.createObject();

  EXPECT_EQ(0, obj.size());
}

TEST(StaticJsonBuffer,
     GivenAJsonObject_WhenValuesAreAdded_ThenSizeIsIncreasedAccordingly) {
  StaticJsonBuffer<SIZEOF_OBJECT_WITH_TWO_VALUES> json;

  JsonObject &obj = json.createObject();
  obj["hello"];
  obj["world"];

  EXPECT_EQ(SIZEOF_OBJECT_WITH_TWO_VALUES, json.size());
}

TEST(
    StaticJsonBuffer,
    GivenAJsonObject_WhenSameValuesAreAddedTwice_ThenSizeIsOnlyIncreasedByTwo) {
  StaticJsonBuffer<SIZEOF_OBJECT_WITH_TWO_VALUES> json;

  JsonObject &obj = json.createObject();
  obj["hello"];
  obj["hello"];

  EXPECT_EQ(SIZEOF_OBJECT_WITH_ONE_VALUE, json.size());
}
