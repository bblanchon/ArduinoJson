// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#define ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson.h>

#define SUITE JsonVariant_Is_Tests

using namespace ArduinoJson::Internals;

template <typename TTo, typename TFrom>
void assertIsNot(TFrom value) {
  JsonVariant variant = value;
  ASSERT_FALSE(variant.is<TTo>());
}

template <typename TTo>
void assertIsNot(JsonArray& value) {
  JsonVariant variant = value;
  ASSERT_FALSE(variant.is<TTo>());
}

template <typename TTo, typename TFrom>
void assertIs(TFrom value) {
  JsonVariant variant = value;
  ASSERT_TRUE(variant.is<TTo>());
}

template <typename TTo>
void assertIs(JsonArray& value) {
  JsonVariant variant = value;
  ASSERT_TRUE(variant.is<TTo>());
}

TEST(SUITE, ArrayIsArry) { assertIs<JsonArray&>(JsonArray::invalid()); }
TEST(SUITE, ArrayIsBool) { assertIsNot<bool>(JsonArray::invalid()); }
TEST(SUITE, ArrayIsDouble) { assertIsNot<double>(JsonArray::invalid()); }
TEST(SUITE, ArrayIsFloat) { assertIsNot<float>(JsonArray::invalid()); }
TEST(SUITE, ArrayIsInt) { assertIsNot<int>(JsonArray::invalid()); }
TEST(SUITE, ArrayIsLong) { assertIsNot<long>(JsonArray::invalid()); }
TEST(SUITE, ArrayIsString) { assertIsNot<const char*>(JsonArray::invalid()); }

TEST(SUITE, BoolIsArray) { assertIsNot<JsonArray&>(true); }
TEST(SUITE, BoolIsBool) { assertIs<bool>(true); }
TEST(SUITE, BoolIsDouble) { assertIsNot<double>(true); }
TEST(SUITE, BoolIsFloat) { assertIsNot<float>(true); }
TEST(SUITE, BoolIsInt) { assertIsNot<int>(true); }
TEST(SUITE, BoolIsLong) { assertIsNot<long>(true); }
TEST(SUITE, BoolIsString) { assertIsNot<const char*>(true); }

TEST(SUITE, DoubleIsArray) { assertIsNot<JsonArray&>(4.2); }
TEST(SUITE, DoubleIsBool) { assertIsNot<bool>(4.2); }
TEST(SUITE, DoubleIsDouble) { assertIs<double>(4.2); }
TEST(SUITE, DoubleIsFloat) { assertIs<float>(4.2); }
TEST(SUITE, DoubleIsInt) { assertIsNot<int>(4.2); }
TEST(SUITE, DoubleIsLong) { assertIsNot<long>(4.2); }
TEST(SUITE, DoubleIsString) { assertIsNot<const char*>(4.2); }

TEST(SUITE, LongIsArray) { assertIsNot<JsonArray&>(42L); }
TEST(SUITE, LongIsBool) { assertIsNot<bool>(42L); }
TEST(SUITE, LongIsDouble) { assertIsNot<double>(42L); }
TEST(SUITE, LongIsFloat) { assertIsNot<float>(42L); }
TEST(SUITE, LongIsInt) { assertIs<int>(42L); }
TEST(SUITE, LongIsLong) { assertIs<long>(42L); }
TEST(SUITE, LongIsString) { assertIsNot<const char*>(42L); }

TEST(SUITE, StringIsArray) { assertIsNot<JsonArray&>("42"); }
TEST(SUITE, StringIsBool) { assertIsNot<bool>("42"); }
TEST(SUITE, StringIsDouble) { assertIsNot<double>("42"); }
TEST(SUITE, StringIsFloat) { assertIsNot<float>("42"); }
TEST(SUITE, StringIsInt) { assertIsNot<int>("42"); }
TEST(SUITE, StringIsLong) { assertIsNot<long>("42"); }
TEST(SUITE, StringIsString) { assertIs<const char*>("42"); }

TEST(SUITE, UnparsedIntIsArra) { assertIsNot<JsonArray&>(Unparsed("42")); }
TEST(SUITE, UnparsedIntIsBool) { assertIsNot<bool>(Unparsed("42")); }
TEST(SUITE, UnparsedIntIsDouble) { assertIsNot<double>(Unparsed("42")); }
TEST(SUITE, UnparsedIntIsFloat) { assertIsNot<float>(Unparsed("42")); }
TEST(SUITE, UnparsedIntIsInt) { assertIs<int>(Unparsed("42")); }
TEST(SUITE, UnparsedIntIsLong) { assertIs<long>(Unparsed("42")); }
TEST(SUITE, UnparsedIntIsString) { assertIsNot<const char*>(Unparsed("42")); }

TEST(SUITE, UnparsedFloatIsBool) { assertIsNot<bool>(Unparsed("4.2e-10")); }
TEST(SUITE, UnparsedFloatIsDouble) { assertIs<double>(Unparsed("4.2e-10")); }
TEST(SUITE, UnparsedFloatIsFloat) { assertIs<float>(Unparsed("4.2e-10")); }
TEST(SUITE, UnparsedFloatIsInt) { assertIsNot<int>(Unparsed("4.2e-10")); }
TEST(SUITE, UnparsedFloatIsLong) { assertIsNot<long>(Unparsed("4.2e-10")); }
TEST(SUITE, UnparsedFloatIsStr) { assertIsNot<const char*>(Unparsed("4.2")); }
