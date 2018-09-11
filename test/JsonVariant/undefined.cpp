// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant undefined") {
  JsonVariant variant;

  SECTION("as<long>()") {
    REQUIRE(0 == variant.as<long>());
  }

  SECTION("as<unsigned>()") {
    REQUIRE(0 == variant.as<unsigned>());
  }

  SECTION("as<char*>()") {
    REQUIRE(0 == variant.as<char*>());
  }

  SECTION("as<double>()") {
    REQUIRE(0 == variant.as<double>());
  }

  SECTION("as<bool>()") {
    REQUIRE(false == variant.as<bool>());
  }

  SECTION("as<JsonArray>()") {
    REQUIRE(variant.as<JsonArray>().isNull());
  }

  SECTION("as<const JsonArray>()") {
    REQUIRE(variant.as<const JsonArray>().isNull());
  }

  SECTION("as<JsonObject>()") {
    REQUIRE(variant.as<JsonObject>().isNull());
  }

  SECTION("as<const JsonObject>()") {
    REQUIRE(variant.as<const JsonObject>().isNull());
  }
}
