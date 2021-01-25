// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
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

  SECTION("works on MemberProxy") {
    JsonObject obj = variant["items"].createNestedObject();
    obj["value"] = 42;

    REQUIRE(variant["items"][0]["value"] == 42);
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

  SECTION("works on MemberProxy") {
    JsonArray arr = variant["items"].createNestedArray();
    arr.add(42);

    REQUIRE(variant["items"][0][0] == 42);
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

  SECTION("works on MemberProxy") {
    JsonObject obj = variant["status"].createNestedObject("weather");
    obj["temp"] = 42;

    REQUIRE(variant["status"]["weather"]["temp"] == 42);
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

  SECTION("works on MemberProxy") {
    JsonArray arr = variant["weather"].createNestedArray("temp");
    arr.add(42);

    REQUIRE(variant["weather"]["temp"][0] == 42);
  }
}
