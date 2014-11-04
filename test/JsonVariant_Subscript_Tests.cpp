// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/StaticJsonBuffer.hpp>
#include <ArduinoJson/JsonArray.hpp>
#include <ArduinoJson/JsonObject.hpp>

using namespace ArduinoJson;

class JsonVariant_Subscript_Tests : public ::testing::Test {
 protected:
  StaticJsonBuffer<200> buffer;
  JsonVariant variant;
};

TEST_F(JsonVariant_Subscript_Tests, Array) {
  JsonArray &array = buffer.createArray();
  array.add("element at index 0");
  array.add("element at index 1");

  variant = array;

  EXPECT_EQ(2, variant.size());
  EXPECT_STREQ("element at index 0", variant[0].asString());
  EXPECT_STREQ("element at index 1", variant[1].asString());
  EXPECT_FALSE(variant[-1].success());
  EXPECT_FALSE(variant[3].success());
}
