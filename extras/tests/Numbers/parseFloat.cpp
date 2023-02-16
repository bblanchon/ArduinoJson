// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_USE_DOUBLE 0
#define ARDUINOJSON_ENABLE_NAN 1
#define ARDUINOJSON_ENABLE_INFINITY 1

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

void checkFloat(const char* input, float expected) {
  CAPTURE(input);
  REQUIRE(parseNumber<float>(input) == Approx(expected));
}

void checkFloatNaN(const char* input) {
  CAPTURE(input);
  float result = parseNumber<float>(input);
  REQUIRE(result != result);
}

void checkFloatInf(const char* input, bool negative) {
  CAPTURE(input);
  float x = parseNumber<float>(input);
  if (negative)
    REQUIRE(x < 0);
  else
    REQUIRE(x > 0);
  REQUIRE(x == x);      // not a NaN
  REQUIRE(x * 2 == x);  // a property of infinity
}

TEST_CASE("parseNumber<float>()") {
  SECTION("Float_Short_NoExponent") {
    checkFloat("3.14", 3.14f);
    checkFloat("-3.14", -3.14f);
    checkFloat("+3.14", +3.14f);
  }

  SECTION("Short_NoDot") {
    checkFloat("1E+38", 1E+38f);
    checkFloat("-1E+38", -1E+38f);
    checkFloat("+1E-38", +1E-38f);
    checkFloat("+1e+38", +1e+38f);
    checkFloat("-1e-38", -1e-38f);
  }

  SECTION("Max") {
    checkFloat("340.2823e+36", 3.402823e+38f);
    checkFloat("34.02823e+37", 3.402823e+38f);
    checkFloat("3.402823e+38", 3.402823e+38f);
    checkFloat("0.3402823e+39", 3.402823e+38f);
    checkFloat("0.03402823e+40", 3.402823e+38f);
    checkFloat("0.003402823e+41", 3.402823e+38f);
  }

  SECTION("VeryLong") {
    checkFloat("0.00000000000000000000000000000001", 1e-32f);
    checkFloat("100000000000000000000000000000000.0", 1e+32f);
    checkFloat(
        "100000000000000000000000000000000.00000000000000000000000000000",
        1e+32f);
  }

  SECTION("MantissaTooLongToFit") {
    checkFloat("0.340282346638528861111111111111", 0.34028234663852886f);
    checkFloat("34028234663852886.11111111111111", 34028234663852886.0f);
    checkFloat("34028234.66385288611111111111111", 34028234.663852886f);

    checkFloat("-0.340282346638528861111111111111", -0.34028234663852886f);
    checkFloat("-34028234663852886.11111111111111", -34028234663852886.0f);
    checkFloat("-34028234.66385288611111111111111", -34028234.663852886f);
  }

  SECTION("ExponentTooBig") {
    checkFloatInf("1e39", false);
    checkFloatInf("-1e39", true);
    checkFloatInf("1e255", false);
    checkFloat("1e-255", 0.0f);
  }

  SECTION("NaN") {
    checkFloatNaN("NaN");
    checkFloatNaN("nan");
  }

  SECTION("Infinity") {
    checkFloatInf("Infinity", false);
    checkFloatInf("+Infinity", false);
    checkFloatInf("-Infinity", true);
    checkFloatInf("inf", false);
    checkFloatInf("+inf", false);
    checkFloatInf("-inf", true);

    checkFloatInf("1e300", false);
    checkFloatInf("-1e300", true);
  }
}
