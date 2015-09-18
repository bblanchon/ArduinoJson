// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <limits.h>  // for LONG_MAX
#define ARDUINOJSON_ENABLE_STD_STREAM
#include <ArduinoJson.h>

#define SUITE Issue90

using namespace ArduinoJson::Internals;

static const char* superLong =
    "12345678901234567890123456789012345678901234567890123456789012345678901234"
    "5678901234567890123456789012345678901234567890123456789012345678901234567";

static const JsonVariant variant = Unparsed(superLong);

TEST(SUITE, IsNotALong) { ASSERT_FALSE(variant.is<long>()); }

TEST(SUITE, AsLong) { ASSERT_EQ(LONG_MAX, variant.as<long>()); }

TEST(SUITE, IsAString) { ASSERT_FALSE(variant.is<const char*>()); }

TEST(SUITE, AsString) { ASSERT_STREQ(superLong, variant.as<const char*>()); }
