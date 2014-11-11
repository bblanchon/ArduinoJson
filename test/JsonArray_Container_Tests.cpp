// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonArray_Container_Tests : public ::testing::Test {
 protected:
  JsonArray_Container_Tests() : array(json.createArray()) {}

  template <typename T>
  void firstMustEqual(T expected) {
    itemMustEqual(0, expected);
  }

  template <typename T>
  void secondMustEqual(T expected) {
    itemMustEqual(1, expected);
  }

  template <typename T>
  void firstMustReference(const T& expected) {
    itemMustReference(0, expected);
  }

  template <typename T>
  void secondMustReference(const T& expected) {
    itemMustReference(1, expected);
  }

  void sizeMustBe(int expected) { EXPECT_EQ(expected, array.size()); }

  StaticJsonBuffer<256> json;
  JsonArray& array;

 private:
  template <typename T>
  void itemMustEqual(int index, T expected) {
    EXPECT_EQ(expected, array[index].as<T>());
  }

  template <typename T>
  void itemMustReference(int index, const T& expected) {
    EXPECT_EQ(&expected, &array[index].as<T&>());
  }
};

TEST_F(JsonArray_Container_Tests, SuccessIsTrue) {
  EXPECT_TRUE(array.success());
}

TEST_F(JsonArray_Container_Tests, InitialSizeIsZero) { sizeMustBe(0); }

TEST_F(JsonArray_Container_Tests, Grow_WhenValuesAreAdded) {
  array.add("hello");
  sizeMustBe(1);

  array.add("world");
  sizeMustBe(2);
}

TEST_F(JsonArray_Container_Tests, CanStoreIntegers) {
  array.add(123);
  array.add(456);

  firstMustEqual(123);
  secondMustEqual(456);
}

TEST_F(JsonArray_Container_Tests, CanStoreDoubles) {
  array.add(123.45);
  array.add(456.78);

  firstMustEqual(123.45);
  secondMustEqual(456.78);
}

TEST_F(JsonArray_Container_Tests, CanStoreBooleans) {
  array.add(true);
  array.add(false);

  firstMustEqual(true);
  secondMustEqual(false);
}

TEST_F(JsonArray_Container_Tests, CanStoreStrings) {
  const char* firstString = "h3110";
  const char* secondString = "w0r1d";

  array.add(firstString);
  array.add(secondString);

  firstMustEqual(firstString);
  secondMustEqual(secondString);
}

TEST_F(JsonArray_Container_Tests, CanStoreNestedArrays) {
  JsonArray& innerarray1 = json.createArray();
  JsonArray& innerarray2 = json.createArray();

  array.add(innerarray1);
  array.add(innerarray2);

  firstMustReference(innerarray1);
  secondMustReference(innerarray2);
}

TEST_F(JsonArray_Container_Tests, CanStoreNestedObjects) {
  JsonObject& innerObject1 = json.createObject();
  JsonObject& innerObject2 = json.createObject();

  array.add(innerObject1);
  array.add(innerObject2);

  firstMustReference(innerObject1);
  secondMustReference(innerObject2);
}

TEST_F(JsonArray_Container_Tests, CanCreateNestedArrays) {
  JsonArray& innerarray1 = array.createNestedArray();
  JsonArray& innerarray2 = array.createNestedArray();

  firstMustReference(innerarray1);
  secondMustReference(innerarray2);
}

TEST_F(JsonArray_Container_Tests, CanCreateNestedObjects) {
  JsonObject& innerObject1 = array.createNestedObject();
  JsonObject& innerObject2 = array.createNestedObject();

  firstMustReference(innerObject1);
  secondMustReference(innerObject2);
}
