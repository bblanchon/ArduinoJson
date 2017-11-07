// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::success()") {
  SECTION("ReturnsFalse_WhenUndefined") {
    JsonVariant variant;
    REQUIRE(false == variant.success());
  }

  SECTION("ReturnsTrue_WhenInteger") {
    JsonVariant variant = 0;
    REQUIRE(true == variant.success());
  }

  SECTION("ReturnsTrue_WhenEmptyArray") {
    DynamicJsonBuffer jsonBuffer;

    JsonVariant variant = jsonBuffer.createArray();
    REQUIRE(true == variant.success());
  }

  SECTION("ReturnsTrue_WhenEmptyObject") {
    DynamicJsonBuffer jsonBuffer;

    JsonVariant variant = jsonBuffer.createObject();
    REQUIRE(true == variant.success());
  }

  SECTION("ReturnsFalse_WhenInvalidArray") {
    JsonVariant variant = JsonArray::invalid();
    REQUIRE(false == variant.success());
  }

  SECTION("ReturnsFalse_WhenInvalidObject") {
    JsonVariant variant = JsonObject::invalid();
    REQUIRE(false == variant.success());
  }
}
