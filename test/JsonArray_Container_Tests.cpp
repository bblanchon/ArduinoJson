// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonValue.hpp>
#include "Printers.hpp"

using namespace ArduinoJson;

class JsonArray_Container_Tests : public ::testing::Test {
 protected:
  virtual void SetUp() { array = json.createArray(); }

  void nodeCountMustBe(int expected) { EXPECT_EQ(expected, json.size()); }

  template <typename T>
  void firstElementMustBe(T expected) {
    elementAtIndexMustBe(0, expected);
  }

  template <typename T>
  void secondElementMustBe(T expected) {
    elementAtIndexMustBe(1, expected);
  }

  void sizeMustBe(int expected) { EXPECT_EQ(expected, array.size()); }

  StaticJsonBuffer<42> json;
  JsonArray array;

 private:
  template <typename T>
  void elementAtIndexMustBe(int index, T expected) {
    EXPECT_EQ(expected, array[index].as<T>());
  }
};

TEST_F(JsonArray_Container_Tests, InitialSizeIsZero) {
  sizeMustBe(0);
  nodeCountMustBe(1);
}

TEST_F(JsonArray_Container_Tests, Grow_WhenValuesAreAdded) {
  array.add("hello");
  sizeMustBe(1);
  nodeCountMustBe(2);

  array.add("world");
  sizeMustBe(2);
  nodeCountMustBe(3);
}

TEST_F(JsonArray_Container_Tests, CanStoreIntegers) {
  array.add(123);
  array.add(456);

  firstElementMustBe(123);
  secondElementMustBe(456);
  nodeCountMustBe(3);
}

TEST_F(JsonArray_Container_Tests, CanStoreDoubles) {
  array.add(123.45);
  array.add(456.78);

  firstElementMustBe(123.45);
  secondElementMustBe(456.78);
  nodeCountMustBe(3);
}

TEST_F(JsonArray_Container_Tests, CanStoreBooleans) {
  array.add(true);
  array.add(false);

  firstElementMustBe(true);
  secondElementMustBe(false);
  nodeCountMustBe(3);
}

TEST_F(JsonArray_Container_Tests, CanStoreStrings) {
  const char* firstString = "h3110";
  const char* secondString = "w0r1d";

  array.add(firstString);
  array.add(secondString);

  firstElementMustBe(firstString);
  secondElementMustBe(secondString);
  nodeCountMustBe(3);
}

TEST_F(JsonArray_Container_Tests, CanStoreNestedArrays) {
  JsonArray innerarray1 = json.createArray();
  JsonArray innerarray2 = json.createArray();

  array.add(innerarray1);
  array.add(innerarray2);

  firstElementMustBe(innerarray1);
  secondElementMustBe(innerarray2);
  nodeCountMustBe(1 + 3 + 3);
}

TEST_F(JsonArray_Container_Tests, CanStoreNestedObjects) {
  JsonObject innerObject1 = json.createObject();
  JsonObject innerObject2 = json.createObject();

  array.add(innerObject1);
  array.add(innerObject2);

  firstElementMustBe(innerObject1);
  secondElementMustBe(innerObject2);
  nodeCountMustBe(1 + 3 + 3);
}

TEST_F(JsonArray_Container_Tests, CanCreateNestedArrays) {
  JsonArray innerarray1 = array.createNestedArray();
  JsonArray innerarray2 = array.createNestedArray();

  firstElementMustBe(innerarray1);
  secondElementMustBe(innerarray2);
  nodeCountMustBe(1 + 1 + 1);
}

TEST_F(JsonArray_Container_Tests, CanCreateNestedObjects) {
  JsonObject innerObject1 = array.createNestedObject();
  JsonObject innerObject2 = array.createNestedObject();

  firstElementMustBe(innerObject1);
  secondElementMustBe(innerObject2);
  nodeCountMustBe(3);
}
