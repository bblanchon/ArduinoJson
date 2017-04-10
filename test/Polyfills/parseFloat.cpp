// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson/Polyfills/parseFloat.hpp>

using namespace ArduinoJson::Polyfills;

struct Polyfills_ParseFloat_Float_Tests : testing::Test {
  void check(const char* input, float expected) {
    float actual = parseFloat<float>(input);
    EXPECT_FLOAT_EQ(expected, actual) << input;
  }

  void checkNaN(const char* input) {
    float result = parseFloat<float>(input);
    EXPECT_TRUE(result != result) << input;
  }

  void checkInf(const char* input, bool negative) {
    float x = parseFloat<float>(input);
    if (negative)
      EXPECT_TRUE(x < 0) << input;
    else
      EXPECT_TRUE(x > 0) << input;
    EXPECT_TRUE(x == x && x * 2 == x) << input;
  }
};
#define TEST_FLOAT(X) TEST_F(Polyfills_ParseFloat_Float_Tests, X)

struct Polyfills_ParseFloat_Double_Tests : testing::Test {
  void check(const char* input, double expected) {
    double actual = parseFloat<double>(input);
    EXPECT_DOUBLE_EQ(expected, actual) << input;
  }

  void checkNaN(const char* input) {
    double result = parseFloat<double>(input);
    EXPECT_TRUE(result != result) << input;
  }

  void checkInf(const char* input, bool negative) {
    double x = parseFloat<double>(input);
    if (negative)
      EXPECT_TRUE(x < 0) << input;
    else
      EXPECT_TRUE(x > 0) << input;
    EXPECT_TRUE(x == x && x * 2 == x) << input;
  }
};
#define TEST_DOUBLE(X) TEST_F(Polyfills_ParseFloat_Double_Tests, X)

TEST_DOUBLE(Null) {
  check(NULL, 0);
}

TEST_FLOAT(Null) {
  check(NULL, 0);
}

TEST_DOUBLE(Short_NoExponent) {
  check("3.14", 3.14);
  check("-3.14", -3.14);
  check("+3.14", +3.14);
}

TEST_FLOAT(Float_Short_NoExponent) {
  check("3.14", 3.14f);
  check("-3.14", -3.14f);
  check("+3.14", +3.14f);
}

TEST_DOUBLE(Short_NoDot) {
  check("1E+308", 1E+308);
  check("-1E+308", -1E+308);
  check("+1E-308", +1E-308);
  check("+1e+308", +1e+308);
  check("-1e-308", -1e-308);
}

TEST_FLOAT(Short_NoDot) {
  check("1E+38", 1E+38f);
  check("-1E+38", -1E+38f);
  check("+1E-38", +1E-38f);
  check("+1e+38", +1e+38f);
  check("-1e-38", -1e-38f);
}

TEST_FLOAT(Max) {
  check("340.2823e+36", 3.402823e+38f);
  check("34.02823e+37", 3.402823e+38f);
  check("3.402823e+38", 3.402823e+38f);
  check("0.3402823e+39", 3.402823e+38f);
  check("00.3402823e+40", 3.402823e+38f);
  check("000.3402823e+41", 3.402823e+38f);
}

TEST_DOUBLE(Max) {
  check(".017976931348623147e+310", 1.7976931348623147e+308);
  check(".17976931348623147e+309", 1.7976931348623147e+308);
  check("1.7976931348623147e+308", 1.7976931348623147e+308);
  check("17.976931348623147e+307", 1.7976931348623147e+308);
  check("179.76931348623147e+306", 1.7976931348623147e+308);
}

TEST_DOUBLE(Min) {
  check(".022250738585072014e-306", 2.2250738585072014e-308);
  check(".22250738585072014e-307", 2.2250738585072014e-308);
  check("2.2250738585072014e-308", 2.2250738585072014e-308);
  check("22.250738585072014e-309", 2.2250738585072014e-308);
  check("222.50738585072014e-310", 2.2250738585072014e-308);
}

TEST_DOUBLE(VeryLong) {
  check("0.00000000000000000000000000000001", 1e-32);
  check("100000000000000000000000000000000.0", 1e+32);
  check("100000000000000000000000000000000.00000000000000000000000000000",
        1e+32);
}

TEST_FLOAT(VeryLong) {
  check("0.00000000000000000000000000000001", 1e-32f);
  check("100000000000000000000000000000000.0", 1e+32f);
  check("100000000000000000000000000000000.00000000000000000000000000000",
        1e+32f);
}

TEST_DOUBLE(MantissaTooLongToFit) {
  check("0.179769313486231571111111111111", 0.17976931348623157);
  check("17976931348623157.11111111111111", 17976931348623157.0);
  check("1797693.134862315711111111111111", 1797693.1348623157);

  check("-0.179769313486231571111111111111", -0.17976931348623157);
  check("-17976931348623157.11111111111111", -17976931348623157.0);
  check("-1797693.134862315711111111111111", -1797693.1348623157);
}

TEST_FLOAT(MantissaTooLongToFit) {
  check("0.340282346638528861111111111111", 0.34028234663852886f);
  check("34028234663852886.11111111111111", 34028234663852886.0f);
  check("34028234.66385288611111111111111", 34028234.663852886f);

  check("-0.340282346638528861111111111111", -0.34028234663852886f);
  check("-34028234663852886.11111111111111", -34028234663852886.0f);
  check("-34028234.66385288611111111111111", -34028234.663852886f);
}

TEST_DOUBLE(ExponentTooBig) {
  checkInf("1e309", false);
  checkInf("-1e309", true);
  checkInf("1e65535", false);
  check("1e-65535", 0.0);
}

TEST_FLOAT(ExponentTooBig) {
  checkInf("1e39", false);
  checkInf("-1e39", true);
  checkInf("1e255", false);
  check("1e-255", 0.0f);
}

TEST_DOUBLE(NaN) {
  checkNaN("NaN");
  checkNaN("nan");
}

TEST_FLOAT(NaN) {
  checkNaN("NaN");
  checkNaN("nan");
}

TEST_FLOAT(Infinity) {
  checkInf("Infinity", false);
  checkInf("+Infinity", false);
  checkInf("-Infinity", true);
  checkInf("inf", false);
  checkInf("+inf", false);
  checkInf("-inf", true);
}
