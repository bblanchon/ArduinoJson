// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Serialization/FloatParts.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("FloatParts<double>") {
  SECTION("1.7976931348623157E+308") {
    FloatParts<double> parts(1.7976931348623157E+308);
    REQUIRE(parts.integral == 1);
    REQUIRE(parts.decimal == 797693135);
    REQUIRE(parts.decimalPlaces == 9);
    REQUIRE(parts.exponent == 308);
  }

  SECTION("4.94065645841247e-324") {
    FloatParts<double> parts(4.94065645841247e-324);
    REQUIRE(parts.integral == 4);
    REQUIRE(parts.decimal == 940656458);
    REQUIRE(parts.decimalPlaces == 9);
    REQUIRE(parts.exponent == -324);
  }
}

TEST_CASE("FloatParts<float>") {
  SECTION("3.4E+38") {
    FloatParts<float> parts(3.4E+38f);
    REQUIRE(parts.integral == 3);
    REQUIRE(parts.decimal == 4);
    REQUIRE(parts.decimalPlaces == 1);
    REQUIRE(parts.exponent == 38);
  }

  SECTION("1.17549435e−38") {
    FloatParts<float> parts(1.17549435e-38f);
    REQUIRE(parts.integral == 1);
    REQUIRE(parts.decimal == 175494);
    REQUIRE(parts.decimalPlaces == 6);
    REQUIRE(parts.exponent == -38);
  }
}
