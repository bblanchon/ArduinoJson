// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::add()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array = _jsonBuffer.createArray();

  SECTION("SizeIncreased_WhenValuesAreAdded") {
    _array.add("hello");
    REQUIRE(1U == _array.size());
  }

  SECTION("StoreInteger") {
    _array.add(123);
    REQUIRE(123 == _array[0].as<int>());
    REQUIRE(_array[0].is<int>());
    REQUIRE_FALSE(_array[0].is<double>());
  }

  SECTION("StoreDouble") {
    _array.add(123.45);
    REQUIRE(123.45 == _array[0].as<double>());
    REQUIRE(_array[0].is<double>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreBoolean") {
    _array.add(true);
    REQUIRE(true == _array[0].as<bool>());
    REQUIRE(_array[0].is<bool>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreString") {
    const char* str = "hello";
    _array.add(str);
    REQUIRE(str == _array[0].as<const char*>());
    REQUIRE(_array[0].is<const char*>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreNestedArray") {
    JsonArray& arr = _jsonBuffer.createArray();

    _array.add(arr);

    REQUIRE(&arr == &_array[0].as<JsonArray&>());
    REQUIRE(_array[0].is<JsonArray&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreNestedObject") {
    JsonObject& obj = _jsonBuffer.createObject();

    _array.add(obj);

    REQUIRE(&obj == &_array[0].as<JsonObject&>());
    REQUIRE(_array[0].is<JsonObject&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreArraySubscript") {
    const char* str = "hello";
    JsonArray& arr = _jsonBuffer.createArray();
    arr.add(str);

    _array.add(arr[0]);

    REQUIRE(str == _array[0]);
  }

  SECTION("StoreObjectSubscript") {
    const char* str = "hello";
    JsonObject& obj = _jsonBuffer.createObject();
    obj["x"] = str;

    _array.add(obj["x"]);

    REQUIRE(str == _array[0]);
  }
}
