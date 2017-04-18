// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonArray::set()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array = _jsonBuffer.createArray();
  _array.add(0);

  SECTION("SizeIsUnchanged") {
    _array.set(0, "hello");
    REQUIRE(1U == _array.size());
  }

  SECTION("StoreInteger") {
    _array.set(0, 123);
    REQUIRE(123 == _array[0].as<int>());
    REQUIRE(_array[0].is<int>());
    REQUIRE_FALSE(_array[0].is<double>());
  }

  SECTION("StoreDouble") {
    _array.set(0, 123.45);
    REQUIRE(123.45 == _array[0].as<double>());
    REQUIRE(_array[0].is<double>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreBoolean") {
    _array.set(0, true);
    REQUIRE(true == _array[0].as<bool>());
    REQUIRE(_array[0].is<bool>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreString") {
    _array.set(0, "hello");
    REQUIRE_THAT(_array[0].as<const char*>(), Equals("hello"));
    REQUIRE(_array[0].is<const char*>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreNestedArray") {
    JsonArray& arr = _jsonBuffer.createArray();

    _array.set(0, arr);

    REQUIRE(&arr == &_array[0].as<JsonArray&>());
    REQUIRE(_array[0].is<JsonArray&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreNestedObject") {
    JsonObject& obj = _jsonBuffer.createObject();

    _array.set(0, obj);

    REQUIRE(&obj == &_array[0].as<JsonObject&>());
    REQUIRE(_array[0].is<JsonObject&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("StoreArraySubscript") {
    JsonArray& arr = _jsonBuffer.createArray();
    arr.add("hello");

    _array.set(0, arr[0]);

    REQUIRE_THAT(_array[0].as<char*>(), Equals("hello"));
  }

  SECTION("StoreObjectSubscript") {
    JsonObject& obj = _jsonBuffer.createObject();
    obj["x"] = "hello";

    _array.set(0, obj["x"]);

    REQUIRE_THAT(_array[0].as<char*>(), Equals("hello"));
  }
}
