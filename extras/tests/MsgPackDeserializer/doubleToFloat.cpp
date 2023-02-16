// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::detail;

template <typename T>
static void check(const char* input, T expected) {
  T actual;
  uint8_t* f = reinterpret_cast<uint8_t*>(&actual);
  const uint8_t* d = reinterpret_cast<const uint8_t*>(input);
  doubleToFloat(d, f);
  fixEndianess(actual);
  CHECK(actual == expected);
}

TEST_CASE("doubleToFloat()") {
  check("\x40\x09\x21\xCA\xC0\x83\x12\x6F", 3.1415f);
  check("\x00\x00\x00\x00\x00\x00\x00\x00", 0.0f);
  check("\x80\x00\x00\x00\x00\x00\x00\x00", -0.0f);
  check("\xC0\x5E\xDC\xCC\xCC\xCC\xCC\xCD", -123.45f);
}
