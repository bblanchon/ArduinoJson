// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

JsonError tryParseArray(const char *json, uint8_t nestingLimit) {
  DynamicJsonArray array;
  return deserializeJson(array, json, nestingLimit);
}

JsonError tryParseObject(const char *json, uint8_t nestingLimit) {
  DynamicJsonObject obj;
  return deserializeJson(obj, json, nestingLimit);
}

TEST_CASE("JsonParser nestingLimit") {
  SECTION("ParseArrayWithNestingLimit0") {
    REQUIRE(tryParseArray("[]", 0) == JsonError::Ok);
    REQUIRE(tryParseArray("[[]]", 0) == JsonError::TooDeep);
  }

  SECTION("ParseArrayWithNestingLimit1") {
    REQUIRE(tryParseArray("[[]]", 1) == JsonError::Ok);
    REQUIRE(tryParseArray("[[[]]]", 1) == JsonError::TooDeep);
  }

  SECTION("ParseArrayWithNestingLimit2") {
    REQUIRE(tryParseArray("[[[]]]", 2) == JsonError::Ok);
    REQUIRE(tryParseArray("[[[[]]]]", 2) == JsonError::TooDeep);
  }

  SECTION("ParseObjectWithNestingLimit0") {
    REQUIRE(tryParseObject("{}", 0) == JsonError::Ok);
    REQUIRE(tryParseObject("{\"key\":{}}", 0) == JsonError::TooDeep);
  }

  SECTION("ParseObjectWithNestingLimit1") {
    REQUIRE(tryParseObject("{\"key\":{}}", 1) == JsonError::Ok);
    REQUIRE(tryParseObject("{\"key\":{\"key\":{}}}", 1) == JsonError::TooDeep);
  }

  SECTION("ParseObjectWithNestingLimit2") {
    REQUIRE(tryParseObject("{\"key\":{\"key\":{}}}", 2) == JsonError::Ok);
    REQUIRE(tryParseObject("{\"key\":{\"key\":{\"key\":{}}}}", 2) ==
            JsonError::TooDeep);
  }
}
