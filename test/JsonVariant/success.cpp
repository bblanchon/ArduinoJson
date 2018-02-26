// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
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
    DynamicJsonArray array;

    JsonVariant variant = array;
    REQUIRE(true == variant.success());
  }

  SECTION("ReturnsTrue_WhenEmptyObject") {
    DynamicJsonObject obj;

    JsonVariant variant = obj;
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
