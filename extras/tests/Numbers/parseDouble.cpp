// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_USE_DOUBLE 1
#define ARDUINOJSON_ENABLE_NAN 1
#define ARDUINOJSON_ENABLE_INFINITY 1

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

void checkDouble(const char* input, double expected) {
  CAPTURE(input);
  REQUIRE(parseNumber<double>(input) == Approx(expected));
}

void checkDoubleNaN(const char* input) {
  CAPTURE(input);
  double result = parseNumber<double>(input);
  REQUIRE(result != result);
}

void checkDoubleInf(const char* input, bool negative) {
  CAPTURE(input);
  double x = parseNumber<double>(input);
  if (negative)
    REQUIRE(x < 0);
  else
    REQUIRE(x > 0);
  REQUIRE(x == x);      // not a NaN
  REQUIRE(x * 2 == x);  // a property of infinity
}

TEST_CASE("parseNumber<double>()") {
  SECTION("Short_NoExponent") {
    checkDouble("3.14", 3.14);
    checkDouble("-3.14", -3.14);
    checkDouble("+3.14", +3.14);
  }

  SECTION("Short_NoDot") {
    checkDouble("1E+308", 1E+308);
    checkDouble("-1E+308", -1E+308);
    checkDouble("+1E-308", +1E-308);
    checkDouble("+1e+308", +1e+308);
    checkDouble("-1e-308", -1e-308);
  }

  SECTION("Max") {
    checkDouble(".017976931348623147e+310", 1.7976931348623147e+308);
    checkDouble(".17976931348623147e+309", 1.7976931348623147e+308);
    checkDouble("1.7976931348623147e+308", 1.7976931348623147e+308);
    checkDouble("17.976931348623147e+307", 1.7976931348623147e+308);
    checkDouble("179.76931348623147e+306", 1.7976931348623147e+308);
  }

  SECTION("Min") {
    checkDouble(".022250738585072014e-306", 2.2250738585072014e-308);
    checkDouble(".22250738585072014e-307", 2.2250738585072014e-308);
    checkDouble("2.2250738585072014e-308", 2.2250738585072014e-308);
    checkDouble("22.250738585072014e-309", 2.2250738585072014e-308);
    checkDouble("222.50738585072014e-310", 2.2250738585072014e-308);
  }

  SECTION("VeryLong") {
    checkDouble("0.00000000000000000000000000000001", 1e-32);
    checkDouble("100000000000000000000000000000000.0", 1e+32);
    checkDouble(
        "100000000000000000000000000000000.00000000000000000000000000000",
        1e+32);
  }

  SECTION("MantissaTooLongToFit") {
    checkDouble("0.179769313486231571111111111111", 0.17976931348623157);
    checkDouble("17976931348623157.11111111111111", 17976931348623157.0);
    checkDouble("1797693.134862315711111111111111", 1797693.1348623157);

    checkDouble("-0.179769313486231571111111111111", -0.17976931348623157);
    checkDouble("-17976931348623157.11111111111111", -17976931348623157.0);
    checkDouble("-1797693.134862315711111111111111", -1797693.1348623157);
  }

  SECTION("ExponentTooBig") {
    checkDoubleInf("1e309", false);
    checkDoubleInf("-1e309", true);
    checkDoubleInf("1e65535", false);
    checkDouble("1e-65535", 0.0);
  }

  SECTION("NaN") {
    checkDoubleNaN("NaN");
    checkDoubleNaN("nan");
  }
}
