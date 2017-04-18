// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::set()") {
  DynamicJsonBuffer jb;
  JsonObject& _object = jb.createObject();

  SECTION("SizeIncreased_WhenValuesAreAdded") {
    _object.set("hello", 42);
    REQUIRE(1 == _object.size());
  }

  SECTION("SizeUntouched_WhenSameValueIsAdded") {
    _object["hello"] = 1;
    _object["hello"] = 2;
    REQUIRE(1 == _object.size());
  }

  SECTION("StoreInteger") {
    _object.set("hello", 123);

    REQUIRE(123 == _object["hello"].as<int>());
    REQUIRE(_object["hello"].is<int>());
    REQUIRE_FALSE(_object["hello"].is<double>());
  }

  SECTION("StoreDouble") {
    _object.set("hello", 123.45);

    REQUIRE(123.45 == _object["hello"].as<double>());
    REQUIRE(_object["hello"].is<double>());
    REQUIRE_FALSE(_object["hello"].is<long>());
  }

  SECTION("StoreDoubleWithDigits") {
    _object.set("hello", 123.45, 2);

    REQUIRE(123.45 == _object["hello"].as<double>());
    REQUIRE(_object["hello"].is<double>());
    REQUIRE_FALSE(_object["hello"].is<long>());
  }

  SECTION("StoreBoolean") {
    _object.set("hello", true);

    REQUIRE(_object["hello"].as<bool>());
    REQUIRE(_object["hello"].is<bool>());
    REQUIRE_FALSE(_object["hello"].is<long>());
  }

  SECTION("StoreString") {
    _object.set("hello", "h3110");

    REQUIRE(std::string("h3110") == _object["hello"].as<const char*>());
    REQUIRE(_object["hello"].is<const char*>());
    REQUIRE_FALSE(_object["hello"].is<long>());
  }

  SECTION("StoreArray") {
    JsonArray& arr = jb.createArray();

    _object.set("hello", arr);

    REQUIRE(&arr == &_object["hello"].as<JsonArray>());
    REQUIRE(_object["hello"].is<JsonArray&>());
    REQUIRE_FALSE(_object["hello"].is<JsonObject&>());
  }

  SECTION("StoreObject") {
    JsonObject& obj = jb.createObject();

    _object.set("hello", obj);

    REQUIRE(&obj == &_object["hello"].as<JsonObject>());
    REQUIRE(_object["hello"].is<JsonObject&>());
    REQUIRE_FALSE(_object["hello"].is<JsonArray&>());
  }

  SECTION("StoreArraySubscript") {
    JsonArray& arr = jb.createArray();
    arr.add(42);

    _object.set("a", arr[0]);

    REQUIRE(42 == _object["a"]);
  }

  SECTION("StoreObjectSubscript") {
    JsonObject& obj = jb.createObject();
    obj.set("x", 42);

    _object.set("a", obj["x"]);

    REQUIRE(42 == _object["a"]);
  }

  SECTION("ShouldReturnTrue_WhenAllocationSucceeds") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 15> jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();

    REQUIRE(true == obj.set(std::string("hello"), std::string("world")));
  }

  SECTION("ShouldReturnFalse_WhenAllocationFails") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 10> jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();

    REQUIRE(false == obj.set(std::string("hello"), std::string("world")));
  }
}
