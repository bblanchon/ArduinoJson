// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson;
using namespace ArduinoJson::detail;

TEST_CASE("Test unsigned integer overflow") {
  VariantData first, second;

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(JsonInteger);

  if (integerSize == 8) {
    parseNumber("18446744073709551615", first);
    parseNumber("18446744073709551616", second);
  } else {
    parseNumber("4294967295", first);
    parseNumber("4294967296", second);
  }

  REQUIRE(first.type() == uint8_t(VALUE_IS_UNSIGNED_INTEGER));
  REQUIRE(second.type() == uint8_t(VALUE_IS_FLOAT));
}

TEST_CASE("Test signed integer overflow") {
  VariantData first, second;

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(JsonInteger);

  if (integerSize == 8) {
    parseNumber("-9223372036854775808", first);
    parseNumber("-9223372036854775809", second);
  } else {
    parseNumber("-2147483648", first);
    parseNumber("-2147483649", second);
  }

  REQUIRE(first.type() == uint8_t(VALUE_IS_SIGNED_INTEGER));
  REQUIRE(second.type() == uint8_t(VALUE_IS_FLOAT));
}

TEST_CASE("Invalid value") {
  VariantData result;

  parseNumber("6a3", result);

  REQUIRE(result.type() == uint8_t(VALUE_IS_NULL));
}
