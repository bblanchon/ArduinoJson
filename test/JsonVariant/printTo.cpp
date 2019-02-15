// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <limits>

void check(JsonVariant variant, const std::string &expected) {
  char buffer[256] = "";
  size_t returnValue = variant.printTo(buffer, sizeof(buffer));
  REQUIRE(expected == buffer);
  REQUIRE(expected.size() == returnValue);
}

TEST_CASE("JsonVariant::printTo()") {
  SECTION("Empty") {
    check(JsonVariant(), "");
  }

  SECTION("Null") {
    check(static_cast<char *>(0), "null");
  }

  SECTION("String") {
    check("hello", "\"hello\"");
  }

  SECTION("Double") {
    check(3.1415927, "3.1415927");
  }

  SECTION("Integer") {
    check(42, "42");
  }

  SECTION("NegativeLong") {
    check(-42, "-42");
  }

  SECTION("UnsignedLong") {
    check(4294967295UL, "4294967295");
  }

  SECTION("True") {
    check(true, "true");
  }

  SECTION("OneFalse") {
    check(false, "false");
  }

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("NegativeInt64") {
    check(-9223372036854775807 - 1, "-9223372036854775808");
  }

  SECTION("PositiveInt64") {
    check(9223372036854775807, "9223372036854775807");
  }

  SECTION("UInt64") {
    check(18446744073709551615U, "18446744073709551615");
  }
#endif
}
