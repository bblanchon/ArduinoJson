// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/Numbers/isInteger.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("isInteger()") {
  SECTION("Null") {
    REQUIRE(isInteger(NULL) == false);
  }

  SECTION("Empty string") {
    REQUIRE(isInteger("") == false);
  }

  SECTION("FloatNotInteger") {
    REQUIRE(isInteger("3.14") == false);
    REQUIRE(isInteger("-3.14") == false);
    REQUIRE(isInteger("+3.14") == false);
  }

  SECTION("Spaces") {
    REQUIRE(isInteger("42 ") == false);
    REQUIRE(isInteger(" 42") == false);
  }

  SECTION("Valid") {
    REQUIRE(isInteger("42") == true);
    REQUIRE(isInteger("-42") == true);
    REQUIRE(isInteger("+42") == true);
  }

  SECTION("ExtraSign") {
    REQUIRE(isInteger("--42") == false);
    REQUIRE(isInteger("++42") == false);
  }
}
