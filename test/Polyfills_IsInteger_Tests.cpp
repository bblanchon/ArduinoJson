// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson/Polyfills/isInteger.hpp>

using namespace ArduinoJson::Polyfills;

struct Polyfills_IsInteger_Tests : testing::Test {
  void check(bool expected, const char* input) {
    bool actual = isInteger(input);
    EXPECT_EQ(expected, actual) << input;
  }
};
#define TEST_(X) TEST_F(Polyfills_IsInteger_Tests, X)

TEST_(Null) {
  check(false, NULL);
}

TEST_(FloatNotInteger) {
  check(false, "3.14");
  check(false, "-3.14");
  check(false, "+3.14");
}

TEST_(Spaces) {
  check(false, "42 ");
  check(false, " 42");
}

TEST_(Valid) {
  check(true, "42");
  check(true, "-42");
  check(true, "+42");
}

TEST_(ExtraSign) {
  check(false, "--42");
  check(false, "++42");
}
