// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#define SHOULD_WORK(expression) REQUIRE(DeserializationError::Ok == expression);
#define SHOULD_FAIL(expression) \
  REQUIRE(DeserializationError::TooDeep == expression);

TEST_CASE("JsonDeserializer nestingLimit") {
  DynamicJsonDocument doc;

  SECTION("limit = 0") {
    doc.nestingLimit = 0;
    SHOULD_WORK(deserializeJson(doc, "\"toto\""));
    SHOULD_WORK(deserializeJson(doc, "123"));
    SHOULD_WORK(deserializeJson(doc, "true"));
    SHOULD_FAIL(deserializeJson(doc, "[]"));
    SHOULD_FAIL(deserializeJson(doc, "{}"));
    SHOULD_FAIL(deserializeJson(doc, "[\"toto\"]"));
    SHOULD_FAIL(deserializeJson(doc, "{\"toto\":1}"));
  }

  SECTION("limit = 1") {
    doc.nestingLimit = 1;
    SHOULD_WORK(deserializeJson(doc, "[\"toto\"]"));
    SHOULD_WORK(deserializeJson(doc, "{\"toto\":1}"));
    SHOULD_FAIL(deserializeJson(doc, "{\"toto\":{}}"));
    SHOULD_FAIL(deserializeJson(doc, "{\"toto\":[]}"));
    SHOULD_FAIL(deserializeJson(doc, "[[\"toto\"]]"));
    SHOULD_FAIL(deserializeJson(doc, "[{\"toto\":1}]"));
  }
}
