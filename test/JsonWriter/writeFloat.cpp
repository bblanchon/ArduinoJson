// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <catch.hpp>
#include <limits>
#include <string>

#include <ArduinoJson/Serialization/JsonWriter.hpp>
#include <ArduinoJson/Serialization/StaticStringBuilder.hpp>

using namespace ArduinoJson::Internals;

void check(const std::string& expected, double input, uint8_t digits = 2) {
  char output[1024];
  StaticStringBuilder sb(output, sizeof(output));
  JsonWriter writer(sb);
  writer.writeFloat(input, digits);
  REQUIRE(output == expected);
  REQUIRE(writer.bytesWritten() == expected.size());
}

TEST_CASE("JsonWriter::writeFloat()") {
  SECTION("NaN") {
    check("NaN", std::numeric_limits<double>::signaling_NaN());
  }

  SECTION("PositiveInfinity") {
    check("Infinity", std::numeric_limits<double>::infinity());
  }

  SECTION("NegativeInfinity") {
    check("-Infinity", -std::numeric_limits<double>::infinity());
  }

  SECTION("Zero") {
    check("0.00", 0);
  }

  SECTION("ZeroDigits_Rounding") {
    check("10", 9.5, 0);
  }

  SECTION("ZeroDigits_NoRounding") {
    check("9", 9.4, 0);
  }

  SECTION("OneDigit_Rounding") {
    check("10.0", 9.95, 1);
  }

  SECTION("OneDigit_NoRounding") {
    check("9.9", 9.94, 1);
  }

  SECTION("TwoDigits_Rounding") {
    check("10.00", 9.995, 2);
  }

  SECTION("TwoDigits_NoRounding") {
    check("9.99", 9.994, 2);
  }

  SECTION("ThreeDigits_Rounding") {
    check("10.000", 9.9995, 3);
  }

  SECTION("ThreeDigits_NoRounding") {
    check("9.999", 9.9994, 3);
  }

  SECTION("FourDigits_Rounding") {
    check("10.0000", 9.99995, 4);
  }

  SECTION("FourDigits_NoRounding") {
    check("9.9999", 9.99994, 4);
  }

  SECTION("FiveDigits_Rounding") {
    check("10.00000", 9.999995, 5);
  }

  SECTION("FiveDigits_NoRounding") {
    check("9.99999", 9.999994, 5);
  }
}
