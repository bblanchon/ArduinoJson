// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/Numbers/isFloat.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("isFloat()") {
  SECTION("Input is NULL") {
    REQUIRE(isFloat(NULL) == false);
  }

  SECTION("Empty string") {
    REQUIRE(isFloat("") == false);
  }

  SECTION("NoExponent") {
    REQUIRE(isFloat("3.14") == true);
    REQUIRE(isFloat("-3.14") == true);
    REQUIRE(isFloat("+3.14") == true);
  }

  SECTION("IntegralPartMissing") {
    REQUIRE(isFloat(".14") == true);
    REQUIRE(isFloat("-.14") == true);
    REQUIRE(isFloat("+.14") == true);
  }

  SECTION("FractionalPartMissing") {
    REQUIRE(isFloat("3.") == true);
    REQUIRE(isFloat("-3.e14") == true);
    REQUIRE(isFloat("+3.e-14") == true);
  }

  SECTION("NoDot") {
    REQUIRE(isFloat("3e14") == true);
    REQUIRE(isFloat("3e-14") == true);
    REQUIRE(isFloat("3e+14") == true);
  }

  SECTION("Integer") {
    REQUIRE(isFloat("14") == true);
    REQUIRE(isFloat("-14") == true);
    REQUIRE(isFloat("+14") == true);
  }

  SECTION("ExponentMissing") {
    REQUIRE(isFloat("3.14e") == false);
    REQUIRE(isFloat("3.14e-") == false);
    REQUIRE(isFloat("3.14e+") == false);
  }

  SECTION("JustASign") {
    REQUIRE(isFloat("-") == false);
    REQUIRE(isFloat("+") == false);
  }

  SECTION("Empty") {
    REQUIRE(isFloat("") == false);
  }

  SECTION("NaN") {
    REQUIRE(isFloat("NaN") == true);
    REQUIRE(isFloat("n") == false);
    REQUIRE(isFloat("N") == false);
    REQUIRE(isFloat("nan") == false);
    REQUIRE(isFloat("-NaN") == false);
    REQUIRE(isFloat("+NaN") == false);
  }

  SECTION("Infinity") {
    REQUIRE(isFloat("Infinity") == true);
    REQUIRE(isFloat("+Infinity") == true);
    REQUIRE(isFloat("-Infinity") == true);
    REQUIRE(isFloat("infinity") == false);
    REQUIRE(isFloat("Inf") == false);
  }
}
