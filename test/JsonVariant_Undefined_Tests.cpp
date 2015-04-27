// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>
#include "Printers.hpp"

class JsonVariant_Undefined_Tests : public ::testing::Test {
 protected:
  JsonVariant variant;
};

TEST_F(JsonVariant_Undefined_Tests, SuccessReturnsFalse) {
  EXPECT_FALSE(variant.success());
}

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
  EXPECT_EQ(JsonArray::invalid(), variant.asArray());
}

TEST_F(JsonVariant_Undefined_Tests, AsObjectReturnInvalid) {
  EXPECT_EQ(JsonObject::invalid(), variant.asObject());
}

TEST_F(JsonVariant_Undefined_Tests, CanBeSetToLong) {
  variant = 0L;
  EXPECT_TRUE(variant.success());
}

TEST_F(JsonVariant_Undefined_Tests, CanBeSetToDouble) {
  variant = 0.0;
  EXPECT_TRUE(variant.success());
}

TEST_F(JsonVariant_Undefined_Tests, CanBeSetToString) {
  variant = static_cast<const char*>(NULL);
  EXPECT_TRUE(variant.success());
}

TEST_F(JsonVariant_Undefined_Tests, CanBeSetToBool) {
  variant = false;
  EXPECT_TRUE(variant.success());
}

TEST_F(JsonVariant_Undefined_Tests, CanBeSetToArray) {
  DynamicJsonBuffer jsonBuffer;
  variant = jsonBuffer.createArray();
  EXPECT_TRUE(variant.success());
}

TEST_F(JsonVariant_Undefined_Tests, CanBeSetToObject) {
  DynamicJsonBuffer jsonBuffer;
  variant = jsonBuffer.createObject();
  EXPECT_TRUE(variant.success());
}
