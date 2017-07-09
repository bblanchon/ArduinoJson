// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson/Polyfills/normalize.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Polyfills;

TEST_CASE("normalize<double>()") {
  SECTION("1.7976931348623157E+308") {
    double value = 1.7976931348623157E+308;
    int exp = normalize(value);
    REQUIRE(value == Approx(1.7976931348623157));
    REQUIRE(exp == 308);
  }

  SECTION("4.94065645841247e-324") {
    double value = 4.94065645841247e-324;
    int exp = normalize(value);
    REQUIRE(value == Approx(4.94065645841247));
    REQUIRE(exp == -324);
  }
}

TEST_CASE("normalize<float>()") {
  SECTION("3.4E+38") {
    float value = 3.4E+38f;
    int exp = normalize(value);
    REQUIRE(value == Approx(3.4f));
    REQUIRE(exp == 38);
  }

  SECTION("1.17549435e−38") {
    float value = 1.17549435e-38f;
    int exp = normalize(value);
    REQUIRE(value == Approx(1.17549435));
    REQUIRE(exp == -38);
  }
}
