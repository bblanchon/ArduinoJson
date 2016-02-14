// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <sstream>
#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(StdStream, JsonVariantFalse) {
  std::ostringstream os;
  JsonVariant variant = false;
  os << variant;
  ASSERT_EQ("false", os.str());
}

TEST(StdStream, JsonVariantString) {
  std::ostringstream os;
  JsonVariant variant = "coucou";
  os << variant;
  ASSERT_EQ("\"coucou\"", os.str());
}

TEST(StdStream, JsonObject) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  object["key"] = "value";
  os << object;
  ASSERT_EQ("{\"key\":\"value\"}", os.str());
}

TEST(StdStream, JsonObjectSubscript) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  object["key"] = "value";
  os << object["key"];
  ASSERT_EQ("\"value\"", os.str());
}

TEST(StdStream, JsonArray) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  array.add("value");
  os << array;
  ASSERT_EQ("[\"value\"]", os.str());
}

TEST(StdStream, JsonArraySubscript) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  array.add("value");
  os << array[0];
  ASSERT_EQ("\"value\"", os.str());
}
