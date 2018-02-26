// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("deserializeJson(JsonVariant&)") {
  DynamicJsonVariant variant;

  SECTION("EmptyObject") {
    bool success = deserializeJson(variant, "{}");

    REQUIRE(success == true);
    REQUIRE(variant.is<JsonObject>());
  }

  SECTION("EmptyArray") {
    bool success = deserializeJson(variant, "[]");

    REQUIRE(success == true);
    REQUIRE(variant.is<JsonArray>());
  }

  SECTION("Integer") {
    bool success = deserializeJson(variant, "-42");

    REQUIRE(success == true);
    REQUIRE(variant.is<int>());
    REQUIRE_FALSE(variant.is<bool>());
    REQUIRE(variant == -42);
  }

  SECTION("Double") {
    bool success = deserializeJson(variant, "-1.23e+4");

    REQUIRE(success == true);
    REQUIRE_FALSE(variant.is<int>());
    REQUIRE(variant.is<double>());
    REQUIRE(variant.as<double>() == Approx(-1.23e+4));
  }

  SECTION("Double quoted string") {
    bool success = deserializeJson(variant, "\"hello world\"");

    REQUIRE(success == true);
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello world"));
  }

  SECTION("Single quoted string") {
    bool success = deserializeJson(variant, "\'hello world\'");

    REQUIRE(success == true);
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello world"));
  }

  SECTION("True") {
    bool success = deserializeJson(variant, "true");

    REQUIRE(success == true);
    REQUIRE(variant.is<bool>());
    REQUIRE(variant == true);
  }

  SECTION("False") {
    bool success = deserializeJson(variant, "false");

    REQUIRE(success == true);
    REQUIRE(variant.is<bool>());
    REQUIRE(variant == false);
  }

  SECTION("OpenBrace") {
    bool success = deserializeJson(variant, "{");

    REQUIRE(success == false);
  }

  SECTION("Incomplete string") {
    bool success = deserializeJson(variant, "\"hello");

    REQUIRE(success == true);
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello"));
  }
}
