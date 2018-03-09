// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("deserializeJson(JsonVariant&)") {
  DynamicJsonVariant variant;

  SECTION("EmptyObject") {
    JsonError err = deserializeJson(variant, "{}");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<JsonObject>());
  }

  SECTION("EmptyArray") {
    JsonError err = deserializeJson(variant, "[]");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<JsonArray>());
  }

  SECTION("Integer") {
    JsonError err = deserializeJson(variant, "-42");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<int>());
    REQUIRE_FALSE(variant.is<bool>());
    REQUIRE(variant == -42);
  }

  SECTION("Double") {
    JsonError err = deserializeJson(variant, "-1.23e+4");

    REQUIRE(err == JsonError::Ok);
    REQUIRE_FALSE(variant.is<int>());
    REQUIRE(variant.is<double>());
    REQUIRE(variant.as<double>() == Approx(-1.23e+4));
  }

  SECTION("Double quoted string") {
    JsonError err = deserializeJson(variant, "\"hello world\"");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello world"));
  }

  SECTION("Single quoted string") {
    JsonError err = deserializeJson(variant, "\'hello world\'");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello world"));
  }

  SECTION("True") {
    JsonError err = deserializeJson(variant, "true");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<bool>());
    REQUIRE(variant == true);
  }

  SECTION("False") {
    JsonError err = deserializeJson(variant, "false");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<bool>());
    REQUIRE(variant == false);
  }

  SECTION("OpenBrace") {
    JsonError err = deserializeJson(variant, "{");

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("Incomplete string") {
    JsonError err = deserializeJson(variant, "\"hello");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello"));
  }
}
