// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

bool tryParse(const char *json, uint8_t nestingLimit) {
  DynamicJsonBuffer buffer;
  return buffer.parse(json, nestingLimit).success();
}

bool tryParseArray(const char *json, uint8_t nestingLimit) {
  DynamicJsonBuffer buffer;
  return buffer.parseArray(json, nestingLimit).success();
}

bool tryParseObject(const char *json, uint8_t nestingLimit) {
  DynamicJsonBuffer buffer;
  return buffer.parseObject(json, nestingLimit).success();
}

TEST_CASE("JsonParser nestingLimit") {
  SECTION("parseArray()") {
    SECTION("limit = 0") {
      REQUIRE(true == tryParseArray("[]", 0));
      REQUIRE(false == tryParseArray("[[]]", 0));
    }

    SECTION("limit = 1") {
      REQUIRE(true == tryParseArray("[[]]", 1));
      REQUIRE(false == tryParseArray("[[[]]]", 1));
    }

    SECTION("limit = 2") {
      REQUIRE(true == tryParseArray("[[[]]]", 2));
      REQUIRE(false == tryParseArray("[[[[]]]]", 2));
    }
  }

  SECTION("parseObject()") {
    SECTION("limit = 0") {
      REQUIRE(true == tryParseObject("{}", 0));
      REQUIRE(false == tryParseObject("{\"key\":{}}", 0));
    }

    SECTION("limit = 1") {
      REQUIRE(true == tryParseObject("{\"key\":{}}", 1));
      REQUIRE(false == tryParseObject("{\"key\":{\"key\":{}}}", 1));
    }

    SECTION("limit = 2") {
      REQUIRE(true == tryParseObject("{\"key\":{\"key\":{}}}", 2));
      REQUIRE(false == tryParseObject("{\"key\":{\"key\":{\"key\":{}}}}", 2));
    }
  }

  SECTION("parse()") {
    SECTION("limit = 0") {
      REQUIRE(true == tryParse("\"toto\"", 0));
      REQUIRE(true == tryParse("[]", 0));
      REQUIRE(true == tryParse("{}", 0));
      REQUIRE(false == tryParse("[\"toto\"]", 0));
      REQUIRE(false == tryParse("{\"toto\":1}", 0));
    }

    SECTION("limit = 1") {
      REQUIRE(true == tryParse("[\"toto\"]", 1));
      REQUIRE(true == tryParse("{\"toto\":1}", 1));
      REQUIRE(false == tryParse("[[\"toto\"]]", 1));
      REQUIRE(false == tryParse("[{\"toto\":1}]", 1));
    }
  }
}
