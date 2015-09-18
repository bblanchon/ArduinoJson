// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#define ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson.h>

static const char* null = 0;

TEST(JsonVariant_As_Tests, DoubleAsBool) {
  JsonVariant variant = 4.2;
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, DoubleAsCstr) {
  JsonVariant variant = 4.2;
  ASSERT_FALSE(variant.as<const char*>());
}

TEST(JsonVariant_As_Tests, DoubleAsString) {
  JsonVariant variant = 4.2;
  ASSERT_EQ(String("4.20"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, DoubleAsLong) {
  JsonVariant variant = 4.2;
  ASSERT_EQ(4L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, DoubleZeroAsBool) {
  JsonVariant variant = 0.0;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, DoubleZeroAsLong) {
  JsonVariant variant = 0.0;
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, FalseAsBool) {
  JsonVariant variant = false;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, FalseAsDouble) {
  JsonVariant variant = false;
  ASSERT_EQ(0.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, FalseAsLong) {
  JsonVariant variant = false;
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, FalseAsString) {
  JsonVariant variant = false;
  ASSERT_EQ(String("0"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, TrueAsBool) {
  JsonVariant variant = true;
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, TrueAsDouble) {
  JsonVariant variant = true;
  ASSERT_EQ(1.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, TrueAsLong) {
  JsonVariant variant = true;
  ASSERT_EQ(1L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, TrueAsString) {
  JsonVariant variant = true;
  ASSERT_EQ(String("1"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, LongAsBool) {
  JsonVariant variant = 42L;
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, LongZeroAsBool) {
  JsonVariant variant = 0L;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, LongAsDouble) {
  JsonVariant variant = 42L;
  ASSERT_EQ(42.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, LongAsString) {
  JsonVariant variant = 42L;
  ASSERT_EQ(String("42"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, LongZeroAsDouble) {
  JsonVariant variant = 0L;
  ASSERT_EQ(0.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, NullAsBool) {
  JsonVariant variant = null;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, NullAsDouble) {
  JsonVariant variant = null;
  ASSERT_EQ(0.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, NullAsLong) {
  JsonVariant variant = null;
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, NullAsString) {
  JsonVariant variant = null;
  ASSERT_EQ(String("null"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, NumberStringAsBool) {
  JsonVariant variant = "42";
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, NumberStringAsLong) {
  JsonVariant variant = "42";
  ASSERT_EQ(42L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, RandomStringAsBool) {
  JsonVariant variant = "hello";
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, RandomStringAsLong) {
  JsonVariant variant = "hello";
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, RandomStringAsString) {
  JsonVariant variant = "hello";
  ASSERT_EQ(String("hello"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, TrueStringAsBool) {
  JsonVariant variant = "true";
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, TrueStringAsLong) {
  JsonVariant variant = "true";
  ASSERT_EQ(1L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, ObjectAsString) {
  DynamicJsonBuffer buffer;

  JsonObject& obj = buffer.createObject();
  obj["key"] = "value";

  JsonVariant variant = obj;
  ASSERT_EQ(String("{\"key\":\"value\"}"), variant.as<String>());
}

TEST(JsonVariant_As_Tests, ArrayAsString) {
  DynamicJsonBuffer buffer;

  JsonArray& arr = buffer.createArray();
  arr.add(4);
  arr.add(2);

  JsonVariant variant = arr;
  ASSERT_EQ(String("[4,2]"), variant.as<String>());
}
