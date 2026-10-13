// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson;
using namespace ArduinoJson::detail;

TEST_CASE("stringToDecimal()") {
  SECTION("Largest possible significant") {
    auto result = stringToDecimal<double>("18446744073709551615");

    CHECK(result.significand == 18446744073709551615ULL);
    CHECK(result.exponent == 0);
  }

  SECTION("Overflow significand") {
    auto result = stringToDecimal<double>("18446744073709551616");

    CHECK(result.significand == 1844674407370955161ULL);
    CHECK(result.exponent == 1);
  }
}
