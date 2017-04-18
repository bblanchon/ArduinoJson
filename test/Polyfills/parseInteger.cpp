// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <stdint.h>
#include <ArduinoJson/Polyfills/parseInteger.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Polyfills;

template <typename T>
void check(const char* input, T expected) {
  CAPTURE(input);
  T actual = parseInteger<T>(input);
  REQUIRE(expected == actual);
}

TEST_CASE("parseInteger<int8_t>()") {
  check<int8_t>("-128", -128);
  check<int8_t>("127", 127);
  check<int8_t>("+127", 127);
  check<int8_t>("3.14", 3);
  // check<int8_t>(" 42", 0);
  check<int8_t>("x42", 0);
  check<int8_t>("128", -128);
  check<int8_t>("-129", 127);
  check<int8_t>(NULL, 0);
}

TEST_CASE("parseInteger<int16_t>()") {
  check<int16_t>("-32768", -32768);
  check<int16_t>("32767", 32767);
  check<int16_t>("+32767", 32767);
  check<int16_t>("3.14", 3);
  // check<int16_t>(" 42", 0);
  check<int16_t>("x42", 0);
  check<int16_t>("-32769", 32767);
  check<int16_t>("32768", -32768);
  check<int16_t>(NULL, 0);
}

TEST_CASE("parseInteger<int32_t>()") {
  check<int32_t>("-2147483648", (-2147483647 - 1));
  check<int32_t>("2147483647", 2147483647);
  check<int32_t>("+2147483647", 2147483647);
  check<int32_t>("3.14", 3);
  // check<int32_t>(" 42", 0);
  check<int32_t>("x42", 0);
  check<int32_t>("-2147483649", 2147483647);
  check<int32_t>("2147483648", (-2147483647 - 1));
}

TEST_CASE("parseInteger<uint8_t>()") {
  check<uint8_t>("0", 0);
  check<uint8_t>("255", 255);
  check<uint8_t>("+255", 255);
  check<uint8_t>("3.14", 3);
  // check<uint8_t>(" 42", 0);
  check<uint8_t>("x42", 0);
  check<uint8_t>("-1", 255);
  check<uint8_t>("256", 0);
}

TEST_CASE("parseInteger<uint16_t>()") {
  check<uint16_t>("0", 0);
  check<uint16_t>("65535", 65535);
  check<uint16_t>("+65535", 65535);
  check<uint16_t>("3.14", 3);
  // check<uint16_t>(" 42", 0);
  check<uint16_t>("x42", 0);
  check<uint16_t>("-1", 65535);
  check<uint16_t>("65536", 0);
}
