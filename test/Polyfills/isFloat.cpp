// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/Polyfills/isFloat.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("isFloat()") {
  SECTION("Input is NULL") {
    REQUIRE(isFloat(NULL) == false);
  }

  SECTION("NoExponent") {
    REQUIRE(isFloat("3.14"));
    REQUIRE(isFloat("-3.14"));
    REQUIRE(isFloat("+3.14"));
  }

  SECTION("IntegralPartMissing") {
    REQUIRE(isFloat(".14"));
    REQUIRE(isFloat("-.14"));
    REQUIRE(isFloat("+.14"));
  }

  SECTION("FractionalPartMissing") {
    REQUIRE(isFloat("3."));
    REQUIRE(isFloat("-3.e14"));
    REQUIRE(isFloat("+3.e-14"));
  }

  SECTION("NoDot") {
    REQUIRE(isFloat("3e14"));
    REQUIRE(isFloat("3e-14"));
    REQUIRE(isFloat("3e+14"));
  }

  SECTION("Integer") {
    REQUIRE(isFloat("14"));
    REQUIRE(isFloat("-14"));
    REQUIRE(isFloat("+14"));
  }

  SECTION("ExponentMissing") {
    REQUIRE_FALSE(isFloat("3.14e"));
    REQUIRE_FALSE(isFloat("3.14e-"));
    REQUIRE_FALSE(isFloat("3.14e+"));
  }

  SECTION("JustASign") {
    REQUIRE_FALSE(isFloat("-"));
    REQUIRE_FALSE(isFloat("+"));
  }

  SECTION("Empty") {
    REQUIRE_FALSE(isFloat(""));
  }

  SECTION("NaN") {
    REQUIRE(isFloat("NaN"));
    REQUIRE_FALSE(isFloat("n"));
    REQUIRE_FALSE(isFloat("N"));
    REQUIRE_FALSE(isFloat("nan"));
    REQUIRE_FALSE(isFloat("-NaN"));
    REQUIRE_FALSE(isFloat("+NaN"));
  }

  SECTION("Infinity") {
    REQUIRE(isFloat("Infinity"));
    REQUIRE(isFloat("+Infinity"));
    REQUIRE(isFloat("-Infinity"));
    REQUIRE_FALSE(isFloat("infinity"));
    REQUIRE_FALSE(isFloat("Inf"));
  }
}
