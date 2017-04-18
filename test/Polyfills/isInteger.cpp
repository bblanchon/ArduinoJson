// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson/Polyfills/isInteger.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Polyfills;

TEST_CASE("isInteger()") {
  SECTION("Null") {
    REQUIRE_FALSE(isInteger(NULL));
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
