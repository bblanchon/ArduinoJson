// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#define SHOULD_WORK(expression) REQUIRE(JsonError::Ok == expression);
#define SHOULD_FAIL(expression) REQUIRE(JsonError::TooDeep == expression);

TEST_CASE("JsonParser nestingLimit") {
  SECTION("deserializeJson(JsonArray&)") {
    DynamicJsonArray arr;

    SECTION("limit = 0") {
      SHOULD_FAIL(deserializeJson(arr, "[]", 0));
    }

    SECTION("limit = 1") {
      SHOULD_WORK(deserializeJson(arr, "[]", 1));
      SHOULD_FAIL(deserializeJson(arr, "[[]]", 1));
    }

    SECTION("limit = 2") {
      SHOULD_WORK(deserializeJson(arr, "[[]]", 2));
      SHOULD_FAIL(deserializeJson(arr, "[[[]]]", 2));
    }
  }

  SECTION("deserializeJson(JsonObject&)") {
    DynamicJsonObject obj;

    SECTION("limit = 0") {
      SHOULD_FAIL(deserializeJson(obj, "{}", 0));
    }

    SECTION("limit = 1") {
      SHOULD_WORK(deserializeJson(obj, "{\"key\":42}", 1));
      SHOULD_FAIL(deserializeJson(obj, "{\"key\":{\"key\":42}}", 1));
    }

    SECTION("limit = 2") {
      SHOULD_WORK(deserializeJson(obj, "{\"key\":{\"key\":42}}", 2));
      SHOULD_FAIL(deserializeJson(obj, "{\"key\":{\"key\":{\"key\":42}}}", 2));
    }
  }

  SECTION("deserializeJson(JsonVariant&)") {
    DynamicJsonVariant var;

    SECTION("limit = 0") {
      SHOULD_WORK(deserializeJson(var, "\"toto\"", 0));
      SHOULD_WORK(deserializeJson(var, "123", 0));
      SHOULD_WORK(deserializeJson(var, "true", 0));
      SHOULD_FAIL(deserializeJson(var, "[]", 0));
      SHOULD_FAIL(deserializeJson(var, "{}", 0));
      SHOULD_FAIL(deserializeJson(var, "[\"toto\"]", 0));
      SHOULD_FAIL(deserializeJson(var, "{\"toto\":1}", 0));
    }

    SECTION("limit = 1") {
      SHOULD_WORK(deserializeJson(var, "[\"toto\"]", 1));
      SHOULD_WORK(deserializeJson(var, "{\"toto\":1}", 1));
      SHOULD_FAIL(deserializeJson(var, "{\"toto\":{}}", 1));
      SHOULD_FAIL(deserializeJson(var, "{\"toto\":[]}", 1));
      SHOULD_FAIL(deserializeJson(var, "[[\"toto\"]]", 1));
      SHOULD_FAIL(deserializeJson(var, "[{\"toto\":1}]", 1));
    }
  }
}
