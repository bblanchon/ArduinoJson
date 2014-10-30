// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/StaticJsonBuffer.hpp>

using namespace ArduinoJson;

TEST(JsonObject_Iterator_Test, SimpleTest) {
  StaticJsonBuffer<256> jsonBuffer;

  JsonObject &object = jsonBuffer.createObject();
  object["ab"] = 12;
  object["cd"] = 34;

  JsonObject::iterator it = object.begin();
  JsonObject::iterator end = object.end();

  ASSERT_NE(end, it);
  EXPECT_STREQ("ab", it->key);
  EXPECT_EQ(12, it->value.as<int>());
  ++it;
  ASSERT_NE(end, it);
  EXPECT_STREQ("cd", it->key);
  EXPECT_EQ(34, it->value.as<int>());
  ++it;
  EXPECT_EQ(object.end(), it);
}
