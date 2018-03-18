// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("deserializeJson(StaticJsonVariant&)") {
  SECTION("Array") {
    StaticJsonVariant<JSON_ARRAY_SIZE(2)> variant;

    char input[] = "[1,2]";
    JsonError err = deserializeJson(variant, input);

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<JsonArray>());
    REQUIRE(variant[0] == 1);
    REQUIRE(variant[1] == 2);
    REQUIRE(variant.memoryUsage() == JSON_ARRAY_SIZE(2));
  }

  SECTION("Should clear the JsonVariant") {
    StaticJsonVariant<JSON_ARRAY_SIZE(2)> variant;
    char input[] = "[1,2]";
    deserializeJson(variant, input);

    JsonError err = deserializeJson(variant, "{}");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<JsonObject>());
    REQUIRE(variant.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
