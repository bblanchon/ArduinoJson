// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::isNull()") {
  SECTION("ReturnsFalse_WhenUndefined") {
    JsonVariant variant;
    REQUIRE(variant.isNull() == true);
  }

  SECTION("ReturnsTrue_WhenInteger") {
    JsonVariant variant = 0;
    REQUIRE(variant.isNull() == false);
  }

  SECTION("ReturnsTrue_WhenEmptyArray") {
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();

    JsonVariant variant = array;
    REQUIRE(variant.isNull() == false);
  }

  SECTION("ReturnsTrue_WhenEmptyObject") {
    DynamicJsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();

    JsonVariant variant = obj;
    REQUIRE(variant.isNull() == false);
  }

  SECTION("ReturnsFalse_WhenInvalidArray") {
    JsonVariant variant = JsonArray();
    REQUIRE(variant.isNull() == true);
  }

  SECTION("ReturnsFalse_WhenInvalidObject") {
    JsonVariant variant = JsonObject();
    REQUIRE(variant.isNull() == true);
  }
}
