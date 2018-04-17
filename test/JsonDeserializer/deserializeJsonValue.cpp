// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("deserializeJson(DynamicJsonDocument&)") {
  DynamicJsonDocument doc;

  SECTION("EmptyObject") {
    JsonError err = deserializeJson(doc, "{}");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<JsonObject>());
  }

  SECTION("EmptyArray") {
    JsonError err = deserializeJson(doc, "[]");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<JsonArray>());
  }

  SECTION("Integer") {
    JsonError err = deserializeJson(doc, "-42");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<int>());
    REQUIRE_FALSE(doc.is<bool>());
    REQUIRE(doc.as<int>() == -42);
  }

  SECTION("Double") {
    JsonError err = deserializeJson(doc, "-1.23e+4");

    REQUIRE(err == JsonError::Ok);
    REQUIRE_FALSE(doc.is<int>());
    REQUIRE(doc.is<double>());
    REQUIRE(doc.as<double>() == Approx(-1.23e+4));
  }

  SECTION("Double quoted string") {
    JsonError err = deserializeJson(doc, "\"hello world\"");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello world"));
  }

  SECTION("Single quoted string") {
    JsonError err = deserializeJson(doc, "\'hello world\'");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello world"));
  }

  SECTION("True") {
    JsonError err = deserializeJson(doc, "true");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == true);
  }

  SECTION("False") {
    JsonError err = deserializeJson(doc, "false");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == false);
  }

  SECTION("OpenBrace") {
    JsonError err = deserializeJson(doc, "{");

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("Incomplete string") {
    JsonError err = deserializeJson(doc, "\"hello");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello"));
  }

  SECTION("Unterminated escape sequence") {
    JsonError err = deserializeJson(doc, "\"\\\0\"");
    REQUIRE(err == JsonError::InvalidInput);
  }

  SECTION("Should clear the JsonVariant") {
    deserializeJson(doc, "[1,2,3]");
    deserializeJson(doc, "{}");

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
