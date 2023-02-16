// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::createNestedObject()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("promotes to array") {
    JsonObject obj = variant.createNestedObject();
    obj["value"] = 42;

    REQUIRE(variant.is<JsonArray>() == true);
    REQUIRE(variant[0]["value"] == 42);
    REQUIRE(obj.isNull() == false);
  }
}

TEST_CASE("JsonVariant::createNestedArray()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("promotes to array") {
    JsonArray arr = variant.createNestedArray();

    REQUIRE(variant.is<JsonArray>() == true);
    REQUIRE(arr.isNull() == false);
  }
}

TEST_CASE("JsonVariant::createNestedObject(key)") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("promotes to object") {
    JsonObject obj = variant.createNestedObject("weather");
    obj["temp"] = 42;

    REQUIRE(variant.is<JsonObject>() == true);
    REQUIRE(variant["weather"]["temp"] == 42);
  }
}

TEST_CASE("JsonVariant::createNestedArray(key)") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("promotes to object") {
    JsonArray arr = variant.createNestedArray("items");

    REQUIRE(variant.is<JsonObject>() == true);
    REQUIRE(arr.isNull() == false);
  }
}
