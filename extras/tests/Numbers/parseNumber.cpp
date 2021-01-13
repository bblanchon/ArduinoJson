// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson/Numbers/Integer.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Variant/VariantImpl.hpp>
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

  REQUIRE(first.type() == uint8_t(VALUE_IS_POSITIVE_INTEGER));
  REQUIRE(second.type() == uint8_t(VALUE_IS_FLOAT));
}

TEST_CASE("Invalid value") {
  VariantData result;
  result.init();

  parseNumber("6a3", result);

  REQUIRE(result.type() == uint8_t(VALUE_IS_NULL));
}
