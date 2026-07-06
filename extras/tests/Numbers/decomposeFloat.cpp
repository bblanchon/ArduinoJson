// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <doctest.h>
#include <ArduinoJson/Numbers/FloatParts.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("decomposeFloat()") {
  SUBCASE("1.7976931348623157E+308") {
    auto parts = decomposeFloat(1.7976931348623157E+308, 9);
    REQUIRE(parts.integral == 1);
    REQUIRE(parts.decimal == 797693135);
    REQUIRE(parts.decimalPlaces == 9);
    REQUIRE(parts.exponent == 308);
  }

  SUBCASE("4.94065645841247e-324") {
    auto parts = decomposeFloat(4.94065645841247e-324, 9);
    REQUIRE(parts.integral == 4);
    REQUIRE(parts.decimal == 940656458);
    REQUIRE(parts.decimalPlaces == 9);
    REQUIRE(parts.exponent == -324);
  }

  SUBCASE("3.4E+38") {
    auto parts = decomposeFloat(3.4E+38f, 6);
    REQUIRE(parts.integral == 3);
    REQUIRE(parts.decimal == 4);
    REQUIRE(parts.decimalPlaces == 1);
    REQUIRE(parts.exponent == 38);
  }

  SUBCASE("1.17549435e−38") {
    auto parts = decomposeFloat(1.17549435e-38f, 6);
    REQUIRE(parts.integral == 1);
    REQUIRE(parts.decimal == 175494);
    REQUIRE(parts.decimalPlaces == 6);
    REQUIRE(parts.exponent == -38);
  }
}
