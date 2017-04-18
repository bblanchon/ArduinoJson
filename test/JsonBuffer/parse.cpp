// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonBuffer::parse()") {
  DynamicJsonBuffer jb;

  SECTION("EmptyObject") {
    JsonVariant variant = jb.parse("{}");
    REQUIRE(variant.success());
    REQUIRE(variant.is<JsonObject>());
  }

  SECTION("EmptyArray") {
    JsonVariant variant = jb.parse("[]");
    REQUIRE(variant.success());
    REQUIRE(variant.is<JsonArray>());
  }

  SECTION("Integer") {
    JsonVariant variant = jb.parse("-42");
    REQUIRE(variant.success());
    REQUIRE(variant.is<int>());
    REQUIRE_FALSE(variant.is<double>());
    REQUIRE(variant == -42);
  }

  SECTION("Double") {
    JsonVariant variant = jb.parse("-1.23e+4");
    REQUIRE(variant.success());
    REQUIRE_FALSE(variant.is<int>());
    REQUIRE(variant.is<double>());
    REQUIRE(variant.as<double>() == Approx(-1.23e+4));
  }

  SECTION("Double quoted string") {
    JsonVariant variant = jb.parse("\"hello world\"");
    REQUIRE(variant.success());
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello world"));
  }

  SECTION("Single quoted string") {
    JsonVariant variant = jb.parse("\'hello world\'");
    REQUIRE(variant.success());
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello world"));
  }

  SECTION("True") {
    JsonVariant variant = jb.parse("true");
    REQUIRE(variant.success());
    REQUIRE(variant.is<bool>());
    REQUIRE(variant == true);
  }

  SECTION("False") {
    JsonVariant variant = jb.parse("false");
    REQUIRE(variant.success());
    REQUIRE(variant.is<bool>());
    REQUIRE(variant == false);
  }

  SECTION("OpenBrace") {
    JsonVariant variant = jb.parse("{");
    REQUIRE_FALSE(variant.success());
  }

  SECTION("Incomplete string") {
    JsonVariant variant = jb.parse("\"hello");
    REQUIRE(variant.success());
    REQUIRE(variant.is<char*>());
    REQUIRE_THAT(variant.as<char*>(), Equals("hello"));
  }
}
