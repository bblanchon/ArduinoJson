// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson/Polyfills/isFloat.hpp>

using namespace ArduinoJson::Polyfills;

struct Polyfills_IsFloat_Tests : testing::Test {
  void check(bool expected, const char* input) {
    bool actual = isFloat(input);
    EXPECT_EQ(expected, actual) << input;
  }
};
#define TEST_(X) TEST_F(Polyfills_IsFloat_Tests, X)

TEST_(Null) {
  check(false, NULL);
}

TEST_(NoExponent) {
  check(true, "3.14");
  check(true, "-3.14");
  check(true, "+3.14");
}

TEST_(IntegralPartMissing) {
  check(true, ".14");
  check(true, "-.14");
  check(true, "+.14");
}

TEST_(FractionalPartMissing) {
  check(true, "3.");
  check(true, "-3.e14");
  check(true, "+3.e-14");
}

TEST_(NoDot) {
  check(true, "3e14");
  check(true, "3e-14");
  check(true, "3e+14");
}

TEST_(Integer) {
  check(false, "14");
  check(false, "-14");
  check(false, "+14");
}

TEST_(ExponentMissing) {
  check(false, "3.14e");
  check(false, "3.14e-");
  check(false, "3.14e+");
}

TEST_(JustASign) {
  check(false, "-");
  check(false, "+");
}

TEST_(Empty) {
  check(false, "");
}

TEST_(NaN) {
  check(true, "NaN");
  check(false, "n");
  check(false, "N");
  check(false, "nan");
  check(false, "-NaN");
  check(false, "+NaN");
}

TEST_(Infinity) {
  check(true, "Infinity");
  check(true, "+Infinity");
  check(true, "-Infinity");
  check(false, "infinity");
  check(false, "Inf");
}
