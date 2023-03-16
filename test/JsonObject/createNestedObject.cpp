// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2023
// MIT License

#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("JsonObject::createNestedObject()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  SECTION("success() should return true if key is non-null") {
    JsonObject& obj = _object.createNestedObject("key");
    REQUIRE(obj.success() == true);
  }

  SECTION("success() should return false if key is null") {
    const char* null = 0;
    JsonObject& obj = _object.createNestedObject(null);
    REQUIRE(obj.success() == false);
  }
}
