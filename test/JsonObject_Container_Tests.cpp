// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonObject_Container_Tests : public ::testing::Test {
 public:
  JsonObject_Container_Tests() : _object(_jsonBuffer.createObject()) {}

 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object;
};

TEST_F(JsonObject_Container_Tests, InitialSizeIsZero) {
  EXPECT_EQ(0, _object.size());
}

TEST_F(JsonObject_Container_Tests, Grow_WhenValuesAreAdded) {
  _object["hello"] = 1;
  EXPECT_EQ(1, _object.size());

  _object.set("world", 2);
  EXPECT_EQ(2, _object.size());
}

TEST_F(JsonObject_Container_Tests, DoNotGrow_WhenSameValueIsAdded) {
  _object["hello"] = 1;
  EXPECT_EQ(1, _object.size());

  _object["hello"] = 2;
  EXPECT_EQ(1, _object.size());
}

TEST_F(JsonObject_Container_Tests, Shrink_WhenValuesAreRemoved) {
  _object["hello"] = 1;
  _object["world"] = 2;

  _object.remove("hello");
  EXPECT_EQ(1, _object.size());

  _object.remove("world");
  EXPECT_EQ(0, _object.size());
}

TEST_F(JsonObject_Container_Tests,
       DoNotShrink_WhenRemoveIsCalledWithAWrongKey) {
  _object["hello"] = 1;
  _object["world"] = 2;

  _object.remove(":-P");

  EXPECT_EQ(2, _object.size());
}

TEST_F(JsonObject_Container_Tests, CanStoreIntegers) {
  _object["hello"] = 123;
  _object.set("world", 456);

  EXPECT_TRUE(_object["hello"].is<int>());
  EXPECT_FALSE(_object["hello"].is<double>());
  EXPECT_EQ(123, _object["hello"].as<int>());
  EXPECT_EQ(456, _object["world"].as<int>());
}

TEST_F(JsonObject_Container_Tests, CanStoreDoubles) {
  _object["hello"] = 123.45;
  _object.set("world", 456.78);

  EXPECT_TRUE(_object["hello"].is<double>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_EQ(123.45, _object["hello"].as<double>());
  EXPECT_EQ(456.78, _object["world"].as<double>());
}

TEST_F(JsonObject_Container_Tests, CanStoreBooleans) {
  _object["hello"] = true;
  _object.set("world", false);

  EXPECT_TRUE(_object["hello"].is<bool>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_TRUE(_object["hello"].as<bool>());
  EXPECT_FALSE(_object["world"].as<bool>());
}

TEST_F(JsonObject_Container_Tests, CanStoreStrings) {
  _object["hello"] = "h3110";
  _object.set("world", "w0r1d");

  EXPECT_TRUE(_object["hello"].is<const char*>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_STREQ("h3110", _object["hello"].as<const char*>());
  EXPECT_STREQ("w0r1d", _object["world"].as<const char*>());
}

TEST_F(JsonObject_Container_Tests, CanStoreArrays) {
  JsonArray& array1 = _jsonBuffer.createArray();
  JsonArray& array2 = _jsonBuffer.createArray();

  _object["hello"] = array1;
  _object.set("world", array2);

  EXPECT_TRUE(_object["hello"].is<JsonArray&>());
  EXPECT_FALSE(_object["hello"].is<JsonObject&>());
  EXPECT_EQ(&array1, &_object["hello"].asArray());
  EXPECT_EQ(&array2, &_object["world"].asArray());
}

TEST_F(JsonObject_Container_Tests, CanStoreObjects) {
  JsonObject& object1 = _jsonBuffer.createObject();
  JsonObject& object2 = _jsonBuffer.createObject();

  _object["hello"] = object1;
  _object.set("world", object2);

  EXPECT_TRUE(_object["hello"].is<JsonObject&>());
  EXPECT_FALSE(_object["hello"].is<JsonArray&>());
  EXPECT_EQ(&object1, &_object["hello"].asObject());
  EXPECT_EQ(&object2, &_object["world"].asObject());
}

TEST_F(JsonObject_Container_Tests, ContainsKeyReturnsFalseForNonExistingKey) {
  EXPECT_FALSE(_object.containsKey("hello"));
}

TEST_F(JsonObject_Container_Tests, ContainsKeyReturnsTrueForDefinedValue) {
  _object.set("hello", 42);
  EXPECT_TRUE(_object.containsKey("hello"));
}
