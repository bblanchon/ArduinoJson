// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

TEST(DynamicJsonBuffer_Array_Tests, GrowsWithArray) {
  DynamicJsonBuffer jsonBuffer;

  JsonArray &array = jsonBuffer.createArray();
  ASSERT_EQ(JSON_ARRAY_SIZE(0), jsonBuffer.size());

  array.add("hello");
  ASSERT_EQ(JSON_ARRAY_SIZE(1), jsonBuffer.size());

  array.add("world");
  ASSERT_EQ(JSON_ARRAY_SIZE(2), jsonBuffer.size());
}

TEST(DynamicJsonBuffer_Array_Tests, CanAdd1000Values) {
  DynamicJsonBuffer jsonBuffer;

  JsonArray &array = jsonBuffer.createArray();

  for (int i = 1; i <= 1000; i++) {
    array.add("hello");
    ASSERT_EQ(array.size(), i);
  }
}
