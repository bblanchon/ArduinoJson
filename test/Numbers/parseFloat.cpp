// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/Numbers/parseFloat.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

template <typename T>
void check(const char* input, T expected) {
  CAPTURE(input);
  REQUIRE(parseFloat<T>(input) == Approx(expected));
}

template <typename T>
void checkNaN(const char* input) {
  CAPTURE(input);
  T result = parseFloat<T>(input);
  REQUIRE(result != result);
}

template <typename T>
void checkInf(const char* input, bool negative) {
  CAPTURE(input);
  T x = parseFloat<T>(input);
  if (negative)
    REQUIRE(x < 0);
  else
    REQUIRE(x > 0);
  REQUIRE(x == x);      // not a NaN
  REQUIRE(x * 2 == x);  // a property of infinity
}

TEST_CASE("parseFloat<float>()") {
  SECTION("Null") {
    check<float>(NULL, 0);
  }

  SECTION("Float_Short_NoExponent") {
    check<float>("3.14", 3.14f);
    check<float>("-3.14", -3.14f);
    check<float>("+3.14", +3.14f);
  }

  SECTION("Short_NoDot") {
    check<float>("1E+38", 1E+38f);
    check<float>("-1E+38", -1E+38f);
    check<float>("+1E-38", +1E-38f);
    check<float>("+1e+38", +1e+38f);
    check<float>("-1e-38", -1e-38f);
  }

  SECTION("Max") {
    check<float>("340.2823e+36", 3.402823e+38f);
    check<float>("34.02823e+37", 3.402823e+38f);
    check<float>("3.402823e+38", 3.402823e+38f);
    check<float>("0.3402823e+39", 3.402823e+38f);
    check<float>("0.03402823e+40", 3.402823e+38f);
    check<float>("0.003402823e+41", 3.402823e+38f);
  }

  SECTION("VeryLong") {
    check<float>("0.00000000000000000000000000000001", 1e-32f);
    check<float>("100000000000000000000000000000000.0", 1e+32f);
    check<float>(
        "100000000000000000000000000000000.00000000000000000000000000000",
        1e+32f);
  }

  SECTION("MantissaTooLongToFit") {
    check<float>("0.340282346638528861111111111111", 0.34028234663852886f);
    check<float>("34028234663852886.11111111111111", 34028234663852886.0f);
    check<float>("34028234.66385288611111111111111", 34028234.663852886f);

    check<float>("-0.340282346638528861111111111111", -0.34028234663852886f);
    check<float>("-34028234663852886.11111111111111", -34028234663852886.0f);
    check<float>("-34028234.66385288611111111111111", -34028234.663852886f);
  }

  SECTION("ExponentTooBig") {
    checkInf<float>("1e39", false);
    checkInf<float>("-1e39", true);
    checkInf<float>("1e255", false);
    check<float>("1e-255", 0.0f);
  }

  SECTION("NaN") {
    checkNaN<float>("NaN");
    checkNaN<float>("nan");
  }

  SECTION("Infinity") {
    checkInf<float>("Infinity", false);
    checkInf<float>("+Infinity", false);
    checkInf<float>("-Infinity", true);
    checkInf<float>("inf", false);
    checkInf<float>("+inf", false);
    checkInf<float>("-inf", true);
  }

  SECTION("Boolean") {
    check<float>("false", 0.0f);
    check<float>("true", 1.0f);
  }
}

TEST_CASE("parseFloat<double>()") {
  SECTION("Null") {
    check<double>(NULL, 0);
  }

  SECTION("Short_NoExponent") {
    check<double>("3.14", 3.14);
    check<double>("-3.14", -3.14);
    check<double>("+3.14", +3.14);
  }

  SECTION("Short_NoDot") {
    check<double>("1E+308", 1E+308);
    check<double>("-1E+308", -1E+308);
    check<double>("+1E-308", +1E-308);
    check<double>("+1e+308", +1e+308);
    check<double>("-1e-308", -1e-308);
  }

  SECTION("Max") {
    check<double>(".017976931348623147e+310", 1.7976931348623147e+308);
    check<double>(".17976931348623147e+309", 1.7976931348623147e+308);
    check<double>("1.7976931348623147e+308", 1.7976931348623147e+308);
    check<double>("17.976931348623147e+307", 1.7976931348623147e+308);
    check<double>("179.76931348623147e+306", 1.7976931348623147e+308);
  }

  SECTION("Min") {
    check<double>(".022250738585072014e-306", 2.2250738585072014e-308);
    check<double>(".22250738585072014e-307", 2.2250738585072014e-308);
    check<double>("2.2250738585072014e-308", 2.2250738585072014e-308);
    check<double>("22.250738585072014e-309", 2.2250738585072014e-308);
    check<double>("222.50738585072014e-310", 2.2250738585072014e-308);
  }

  SECTION("VeryLong") {
    check<double>("0.00000000000000000000000000000001", 1e-32);
    check<double>("100000000000000000000000000000000.0", 1e+32);
    check<double>(
        "100000000000000000000000000000000.00000000000000000000000000000",
        1e+32);
  }

  SECTION("MantissaTooLongToFit") {
    check<double>("0.179769313486231571111111111111", 0.17976931348623157);
    check<double>("17976931348623157.11111111111111", 17976931348623157.0);
    check<double>("1797693.134862315711111111111111", 1797693.1348623157);

    check<double>("-0.179769313486231571111111111111", -0.17976931348623157);
    check<double>("-17976931348623157.11111111111111", -17976931348623157.0);
    check<double>("-1797693.134862315711111111111111", -1797693.1348623157);
  }

  SECTION("ExponentTooBig") {
    checkInf<double>("1e309", false);
    checkInf<double>("-1e309", true);
    checkInf<double>("1e65535", false);
    check<double>("1e-65535", 0.0);
  }

  SECTION("NaN") {
    checkNaN<double>("NaN");
    checkNaN<double>("nan");
  }

  SECTION("Boolean") {
    check<double>("false", 0.0);
    check<double>("true", 1.0);
  }
}
