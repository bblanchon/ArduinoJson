// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("Test unsigned integer overflow") {
  VariantData first, second;
  first.init();
  second.init();

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(Integer);

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
  first.init();
  second.init();

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(Integer);

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
  result.init();

  parseNumber("6a3", result);

  REQUIRE(result.type() == uint8_t(VALUE_IS_NULL));
}
