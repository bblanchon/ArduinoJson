// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Polyfills/isInteger.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("isInteger()") {
  SECTION("Null") {
    REQUIRE_FALSE(isInteger(NULL));
  }

  SECTION("Empty String") {
    REQUIRE_FALSE(isInteger(""));
  }

  SECTION("FloatNotInteger") {
    REQUIRE_FALSE(isInteger("3.14"));
    REQUIRE_FALSE(isInteger("-3.14"));
    REQUIRE_FALSE(isInteger("+3.14"));
  }

  SECTION("Spaces") {
    REQUIRE_FALSE(isInteger("42 "));
    REQUIRE_FALSE(isInteger(" 42"));
  }

  SECTION("Valid") {
    REQUIRE(isInteger("42"));
    REQUIRE(isInteger("-42"));
    REQUIRE(isInteger("+42"));
  }

  SECTION("ExtraSign") {
    REQUIRE_FALSE(isInteger("--42"));
    REQUIRE_FALSE(isInteger("++42"));
  }
}
