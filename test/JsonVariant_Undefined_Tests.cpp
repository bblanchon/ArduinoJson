// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#define ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson.h>

class JsonVariant_Undefined_Tests : public ::testing::Test {
 protected:
  JsonVariant variant;
};

TEST_F(JsonVariant_Undefined_Tests, AsLongReturns0) {
  EXPECT_EQ(0, variant.as<long>());
}

TEST_F(JsonVariant_Undefined_Tests, AsStringReturnsNull) {
  EXPECT_EQ(0, variant.asString());
}

TEST_F(JsonVariant_Undefined_Tests, AsDoubleReturns0) {
  EXPECT_EQ(0, variant.as<double>());
}

TEST_F(JsonVariant_Undefined_Tests, AsBoolReturnsFalse) {
  EXPECT_FALSE(variant.as<bool>());
}

TEST_F(JsonVariant_Undefined_Tests, AsArrayReturnInvalid) {
  EXPECT_EQ(JsonArray::invalid(), variant.as<JsonArray&>());
}

TEST_F(JsonVariant_Undefined_Tests, AsConstArrayReturnInvalid) {
  EXPECT_EQ(JsonArray::invalid(), variant.as<const JsonArray&>());
}

TEST_F(JsonVariant_Undefined_Tests, AsObjectReturnInvalid) {
  EXPECT_EQ(JsonObject::invalid(), variant.as<JsonObject&>());
}

TEST_F(JsonVariant_Undefined_Tests, AsConstObjectReturnInvalid) {
  EXPECT_EQ(JsonObject::invalid(), variant.as<const JsonObject&>());
}

TEST_F(JsonVariant_Undefined_Tests, AsArrayWrapperReturnInvalid) {
  EXPECT_EQ(JsonArray::invalid(), variant.asArray());
}

TEST_F(JsonVariant_Undefined_Tests, AsObjectWrapperReturnInvalid) {
  EXPECT_EQ(JsonObject::invalid(), variant.asObject());
}
