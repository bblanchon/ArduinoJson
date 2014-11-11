// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class JsonVariant_Copy_Tests : public ::testing::Test {
 protected:
  StaticJsonBuffer<200> json;
  JsonVariant variant1;
  JsonVariant variant2;
};

TEST_F(JsonVariant_Copy_Tests, IntegersAreCopiedByValue) {
  variant1 = 123;
  variant2 = variant1;
  variant1 = 456;

  EXPECT_EQ(123, variant2.as<int>());
}

TEST_F(JsonVariant_Copy_Tests, DoublesAreCopiedByValue) {
  variant1 = 123.45;
  variant2 = variant1;
  variant1 = 456.78;

  EXPECT_EQ(123.45, variant2.as<double>());
}

TEST_F(JsonVariant_Copy_Tests, BooleansAreCopiedByValue) {
  variant1 = true;
  variant2 = variant1;
  variant1 = false;

  EXPECT_TRUE(variant2.as<bool>());
}

TEST_F(JsonVariant_Copy_Tests, StringsAreCopiedByValue) {
  variant1 = "hello";
  variant2 = variant1;
  variant1 = "world";

  EXPECT_STREQ("hello", variant2.as<const char *>());
}

TEST_F(JsonVariant_Copy_Tests, ObjectsAreCopiedByReference) {
  JsonObject &object = json.createObject();

  variant1 = object;

  object["hello"] = "world";

  EXPECT_EQ(1, variant1.asObject().size());
}

TEST_F(JsonVariant_Copy_Tests, ArraysAreCopiedByReference) {
  JsonArray &array = json.createArray();

  variant1 = array;

  array.add("world");

  EXPECT_EQ(1, variant1.asArray().size());
}
