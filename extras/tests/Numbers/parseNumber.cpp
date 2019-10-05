// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("Test uint32_t overflow") {
  ParsedNumber<float, uint32_t> first =
      parseNumber<float, uint32_t>("4294967295");
  ParsedNumber<float, uint32_t> second =
      parseNumber<float, uint32_t>("4294967296");

  REQUIRE(first.type() == uint8_t(VALUE_IS_POSITIVE_INTEGER));
  REQUIRE(second.type() == uint8_t(VALUE_IS_FLOAT));
}

TEST_CASE("Invalid value") {
  ParsedNumber<float, uint32_t> result = parseNumber<float, uint32_t>("6a3");

  REQUIRE(result.type() == uint8_t(VALUE_IS_NULL));
}
