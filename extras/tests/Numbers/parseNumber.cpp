// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson;
using namespace ArduinoJson::detail;

TEST_CASE("Test unsigned integer overflow") {
  Number first, second;

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(JsonInteger);

  if (integerSize == 8) {
    first = parseNumber("18446744073709551615");
    second = parseNumber("18446744073709551616");
  } else {
    first = parseNumber("4294967295");
    second = parseNumber("4294967296");
  }

  REQUIRE(first.type() == NumberType::UnsignedInteger);
  REQUIRE(second.type() == NumberType::Double);
}

TEST_CASE("Test signed integer overflow") {
  Number first, second;

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(JsonInteger);

  if (integerSize == 8) {
    first = parseNumber("-9223372036854775808");
    second = parseNumber("-9223372036854775809");
  } else {
    first = parseNumber("-2147483648");
    second = parseNumber("-2147483649");
  }

  REQUIRE(first.type() == NumberType::SignedInteger);
  REQUIRE(second.type() == NumberType::Double);
}

TEST_CASE("Invalid value") {
  auto result = parseNumber("6a3");

  REQUIRE(result.type() == NumberType::Invalid);
}

TEST_CASE("float") {
  auto result = parseNumber("3.402823e38");

  REQUIRE(result.type() == NumberType::Float);
}

TEST_CASE("double") {
  auto result = parseNumber("1.7976931348623157e308");

  REQUIRE(result.type() == NumberType::Double);
}
