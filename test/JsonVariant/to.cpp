// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

static const char* null = 0;

TEST_CASE("JsonVariant::to<T>() releases string memory") {
  DynamicJsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  variant.set(std::string("hello"));
  REQUIRE(doc.memoryUsage() == JSON_STRING_SIZE(6));

  SECTION("JsonVariant") {
    variant.to<JsonVariant>();
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("JsonArray") {
    variant.to<JsonArray>();
    REQUIRE(doc.memoryUsage() == 0);
  }

  SECTION("JsonObject") {
    variant.to<JsonObject>();
    REQUIRE(doc.memoryUsage() == 0);
  }
}
