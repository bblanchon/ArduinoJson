// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#define SHOULD_WORK(expression) REQUIRE(JsonError::Ok == expression);
#define SHOULD_FAIL(expression) REQUIRE(JsonError::TooDeep == expression);

TEST_CASE("JsonDeserializer nestingLimit") {
  DynamicJsonDocument doc;

  SECTION("limit = 0") {
    SHOULD_WORK(deserializeJson(doc, "\"toto\"", 0));
    SHOULD_WORK(deserializeJson(doc, "123", 0));
    SHOULD_WORK(deserializeJson(doc, "true", 0));
    SHOULD_FAIL(deserializeJson(doc, "[]", 0));
    SHOULD_FAIL(deserializeJson(doc, "{}", 0));
    SHOULD_FAIL(deserializeJson(doc, "[\"toto\"]", 0));
    SHOULD_FAIL(deserializeJson(doc, "{\"toto\":1}", 0));
  }

  SECTION("limit = 1") {
    SHOULD_WORK(deserializeJson(doc, "[\"toto\"]", 1));
    SHOULD_WORK(deserializeJson(doc, "{\"toto\":1}", 1));
    SHOULD_FAIL(deserializeJson(doc, "{\"toto\":{}}", 1));
    SHOULD_FAIL(deserializeJson(doc, "{\"toto\":[]}", 1));
    SHOULD_FAIL(deserializeJson(doc, "[[\"toto\"]]", 1));
    SHOULD_FAIL(deserializeJson(doc, "[{\"toto\":1}]", 1));
  }
}
