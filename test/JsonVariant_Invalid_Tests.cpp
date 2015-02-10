// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>
#include "Printers.hpp"

class JsonVariant_Invalid_Tests : public ::testing::Test {
 public:
  JsonVariant_Invalid_Tests() : variant(JsonVariant::invalid()) {}

 protected:
  JsonVariant variant;
};

TEST_F(JsonVariant_Invalid_Tests, SuccessReturnsFalse) {
  EXPECT_FALSE(variant.success());
}

TEST_F(JsonVariant_Invalid_Tests, AsLongReturns0) {
  EXPECT_EQ(0, variant.as<long>());
}

TEST_F(JsonVariant_Invalid_Tests, AsStringReturnsNull) {
  EXPECT_EQ(0, variant.asString());
}

TEST_F(JsonVariant_Invalid_Tests, AsDoubleReturns0) {
  EXPECT_EQ(0, variant.as<double>());
}

TEST_F(JsonVariant_Invalid_Tests, AsBoolReturnsFalse) {
  EXPECT_FALSE(variant.as<bool>());
}

TEST_F(JsonVariant_Invalid_Tests, AsArrayReturnInvalid) {
  EXPECT_EQ(JsonArray::invalid(), variant.asArray());
}

TEST_F(JsonVariant_Invalid_Tests, AsObjectReturnInvalid) {
  EXPECT_EQ(JsonObject::invalid(), variant.asObject());
}

TEST_F(JsonVariant_Invalid_Tests, CanNotBeSetToLong) {
  variant = 0L;
  EXPECT_FALSE(variant.success());
}

TEST_F(JsonVariant_Invalid_Tests, CanNotBeSetToDouble) {
  variant = 0.0;
  EXPECT_FALSE(variant.success());
}

TEST_F(JsonVariant_Invalid_Tests, CanNotBeSetToString) {
  variant = static_cast<const char*>(NULL);
  EXPECT_FALSE(variant.success());
}

TEST_F(JsonVariant_Invalid_Tests, CanNotBeSetToBool) {
  variant = false;
  EXPECT_FALSE(variant.success());
}

TEST_F(JsonVariant_Invalid_Tests, CanNotBeSetToArray) {
  variant = JsonArray::invalid();
  EXPECT_FALSE(variant.success());
}

TEST_F(JsonVariant_Invalid_Tests, CanNotBeSetToObject) {
  variant = JsonObject::invalid();
  EXPECT_FALSE(variant.success());
}
