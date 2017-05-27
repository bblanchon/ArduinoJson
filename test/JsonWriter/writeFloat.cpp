// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <catch.hpp>
#include <limits>
#include <string>

#include <ArduinoJson/Serialization/DynamicStringBuilder.hpp>
#include <ArduinoJson/Serialization/JsonWriter.hpp>

using namespace ArduinoJson::Internals;

void check(double input, const std::string& expected) {
  std::string output;
  DynamicStringBuilder<std::string> sb(output);
  JsonWriter<DynamicStringBuilder<std::string> > writer(sb);
  writer.writeFloat(input);
  REQUIRE(writer.bytesWritten() == output.size());
  CHECK(expected == output);
}

TEST_CASE("JsonWriter::writeFloat()") {
  SECTION("Pi") {
    check(3.14159265359, "3.141592654");
  }

  SECTION("Signaling NaN") {
    double nan = std::numeric_limits<double>::signaling_NaN();
    check(nan, "NaN");
  }

  SECTION("Quiet NaN") {
    double nan = std::numeric_limits<double>::quiet_NaN();
    check(nan, "NaN");
  }

  SECTION("Infinity") {
    double inf = std::numeric_limits<double>::infinity();
    check(inf, "Infinity");
    check(-inf, "-Infinity");
  }

  SECTION("Zero") {
    check(0.0, "0");
    check(-0.0, "0");
  }

  SECTION("Espilon") {
    check(2.2250738585072014E-308, "2.225073859e-308");
    check(-2.2250738585072014E-308, "-2.225073859e-308");
  }

  SECTION("Max double") {
    check(1.7976931348623157E+308, "1.797693135e308");
    check(-1.7976931348623157E+308, "-1.797693135e308");
  }

  SECTION("Big exponent") {
    // this test increases coverage of normalize()
    check(1e255, "1e255");
    check(1e-255, "1e-255");
  }

  SECTION("Exponentation when <= 1e-5") {
    check(1e-4, "0.0001");
    check(1e-5, "1e-5");

    check(-1e-4, "-0.0001");
    check(-1e-5, "-1e-5");
  }

  SECTION("Exponentation when >= 1e7") {
    check(9999999.999, "9999999.999");
    check(10000000, "1e7");

    check(-9999999.999, "-9999999.999");
    check(-10000000, "-1e7");
  }

  SECTION("Rounding when too many decimals") {
    check(0.000099999999999, "0.0001");
    check(0.0000099999999999, "1e-5");
    check(0.9999999996, "1");
  }

  SECTION("9 decimal places") {
    check(0.100000001, "0.100000001");
    check(0.999999999, "0.999999999");

    check(9.000000001, "9.000000001");
    check(9.999999999, "9.999999999");
  }

  SECTION("10 decimal places") {
    check(0.1000000001, "0.1");
    check(0.9999999999, "1");

    check(9.0000000001, "9");
    check(9.9999999999, "10");
  }
}
