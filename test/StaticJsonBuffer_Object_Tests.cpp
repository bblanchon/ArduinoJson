// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

const size_t SIZEOF_ONE_OBJECT = JSON_OBJECT_SIZE(0);
const size_t SIZEOF_TWO_OBJECTS = 2 * SIZEOF_ONE_OBJECT;
const size_t SIZEOF_OBJECT_WITH_ONE_VALUE = JSON_OBJECT_SIZE(1);
const size_t SIZEOF_OBJECT_WITH_TWO_VALUES = JSON_OBJECT_SIZE(2);

TEST(StaticJsonBuffer_Object_Tests, CapacityMatchTemplateParameter) {
  StaticJsonBuffer<42> json;

  EXPECT_EQ(42, json.capacity());
}

TEST(StaticJsonBuffer_Object_Tests, InitialSizeIsZero) {
  StaticJsonBuffer<42> json;

  EXPECT_EQ(0, json.size());
}

TEST(StaticJsonBuffer_Object_Tests,
     WhenCreateObjectIsCalled_ThenSizeIsIncreasedSizeOfJsonObject) {
  StaticJsonBuffer<SIZEOF_TWO_OBJECTS> json;

  json.createObject();
  json.createObject();
  EXPECT_EQ(SIZEOF_TWO_OBJECTS, json.size());
}

TEST(StaticJsonBuffer_Object_Tests,
     GivenBufferIsFull_WhenCreateObjectIsCalled_ThenSizeDoesNotChange) {
  StaticJsonBuffer<SIZEOF_ONE_OBJECT> json;

  json.createObject();
  json.createObject();
  EXPECT_EQ(SIZEOF_ONE_OBJECT, json.size());
}

TEST(StaticJsonBuffer_Object_Tests,
     GivenEnoughSpace_WhenCreateObjectIsCalled_ThenSuccessIsTrue) {
  StaticJsonBuffer<SIZEOF_ONE_OBJECT> json;

  JsonObject &object = json.createObject();
  EXPECT_TRUE(object.success());
}

TEST(StaticJsonBuffer_Object_Tests,
     GivenNotEnoughSpace_WhenCreateObjectIsCalled_ThenSuccessIsFalse) {
  StaticJsonBuffer<SIZEOF_ONE_OBJECT - 1> json;

  JsonObject &object = json.createObject();
  EXPECT_FALSE(object.success());
}

TEST(StaticJsonBuffer_Object_Tests,
     WhenCreateObjectIsCalled_ThenAnEmptyJsonObjectIsReturned) {
  StaticJsonBuffer<SIZEOF_ONE_OBJECT> json;

  JsonObject &obj = json.createObject();

  EXPECT_EQ(0, obj.size());
}

TEST(StaticJsonBuffer_Object_Tests,
     GivenAJsonObject_WhenValuesAreAdded_ThenSizeIsIncreasedAccordingly) {
  StaticJsonBuffer<SIZEOF_OBJECT_WITH_TWO_VALUES> json;

  JsonObject &obj = json.createObject();
  obj["hello"];
  obj["world"];

  EXPECT_EQ(SIZEOF_OBJECT_WITH_TWO_VALUES, json.size());
}

TEST(
    StaticJsonBuffer_Object_Tests,
    GivenAJsonObject_WhenSameValuesAreAddedTwice_ThenSizeIsOnlyIncreasedByTwo) {
  StaticJsonBuffer<SIZEOF_OBJECT_WITH_TWO_VALUES> json;

  JsonObject &obj = json.createObject();
  obj["hello"];
  obj["hello"];

  EXPECT_EQ(SIZEOF_OBJECT_WITH_ONE_VALUE, json.size());
}
