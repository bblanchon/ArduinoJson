// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

template <typename TArray>
static void run_iterator_test() {
  StaticJsonBuffer<100> jsonBuffer;

  JsonArray &array = jsonBuffer.createArray();
  array.add(12);
  array.add(34);

  typename TArray::iterator it = array.begin();
  typename TArray::iterator end = array.end();

  EXPECT_NE(end, it);
  EXPECT_EQ(12, it->template as<int>());
  EXPECT_EQ(12, static_cast<int>(*it));
  ++it;
  EXPECT_NE(end, it);
  EXPECT_EQ(34, it->template as<int>());
  EXPECT_EQ(34, static_cast<int>(*it));
  ++it;
  EXPECT_EQ(end, it);
}

TEST(JsonArray_Iterator_Test, RunItertorToEnd) {
  run_iterator_test<JsonArray>();
}

TEST(JsonArray_Iterator_Test, RunConstItertorToEnd) {
  run_iterator_test<const JsonArray>();
}
