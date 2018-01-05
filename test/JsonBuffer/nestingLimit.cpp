// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

bool tryParseArray(const char *json, uint8_t nestingLimit) {
  DynamicJsonBuffer buffer;
  return buffer.parseArray(json, nestingLimit).success();
}

bool tryParseObject(const char *json, uint8_t nestingLimit) {
  DynamicJsonBuffer buffer;
  return buffer.parseObject(json, nestingLimit).success();
}

TEST_CASE("JsonParser nestingLimit") {
  SECTION("ParseArrayWithNestingLimit0") {
    REQUIRE(true == tryParseArray("[]", 0));
    REQUIRE(false == tryParseArray("[[]]", 0));
  }

  SECTION("ParseArrayWithNestingLimit1") {
    REQUIRE(true == tryParseArray("[[]]", 1));
    REQUIRE(false == tryParseArray("[[[]]]", 1));
  }

  SECTION("ParseArrayWithNestingLimit2") {
    REQUIRE(true == tryParseArray("[[[]]]", 2));
    REQUIRE(false == tryParseArray("[[[[]]]]", 2));
  }

  SECTION("ParseObjectWithNestingLimit0") {
    REQUIRE(true == tryParseObject("{}", 0));
    REQUIRE(false == tryParseObject("{\"key\":{}}", 0));
  }

  SECTION("ParseObjectWithNestingLimit1") {
    REQUIRE(true == tryParseObject("{\"key\":{}}", 1));
    REQUIRE(false == tryParseObject("{\"key\":{\"key\":{}}}", 1));
  }

  SECTION("ParseObjectWithNestingLimit2") {
    REQUIRE(true == tryParseObject("{\"key\":{\"key\":{}}}", 2));
    REQUIRE(false == tryParseObject("{\"key\":{\"key\":{\"key\":{}}}}", 2));
  }
}
