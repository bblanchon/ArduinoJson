// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(JsonArray_Iterator_Test, SimpleTest) {
  StaticJsonBuffer<100> jsonBuffer;

  JsonArray &array = jsonBuffer.createArray();
  array.add(12);
  array.add(34);

  JsonArray::iterator it = array.begin();
  JsonArray::iterator end = array.end();

  EXPECT_NE(end, it);
  EXPECT_EQ(12, it->as<int>());
  ++it;
  EXPECT_NE(end, it);
  EXPECT_EQ(34, it->as<int>());
  ++it;
  EXPECT_EQ(end, it);
}
