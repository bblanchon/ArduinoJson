// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2023
// MIT License

#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("JsonObject::createNestedArray()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  SECTION("success() should return true if key is non-null") {
    JsonArray& arr = _object.createNestedArray("key");
    REQUIRE(arr.success() == true);
  }

  SECTION("success() should return false if key is null") {
    const char* null = 0;
    JsonArray& arr = _object.createNestedArray(null);
    REQUIRE(arr.success() == false);
  }
}
