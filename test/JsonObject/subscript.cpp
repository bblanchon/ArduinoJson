// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::operator[]") {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  SECTION("SizeIncreased_WhenValuesAreAdded") {
    _object["hello"] = 1;
    REQUIRE(1 == _object.size());
  }

  SECTION("SizeUntouched_WhenSameValueIsAdded") {
    _object["hello"] = 1;
    _object["hello"] = 2;
    REQUIRE(1 == _object.size());
  }

  SECTION("StoreInteger") {
    _object["hello"] = 123;

    REQUIRE(123 == _object["hello"].as<int>());
    REQUIRE(true == _object["hello"].is<int>());
    REQUIRE(false == _object["hello"].is<double>());
  }

  SECTION("StoreVolatileInteger") {  // issue #415
    volatile int i = 123;
    _object["hello"] = i;

    REQUIRE(123 == _object["hello"].as<int>());
    REQUIRE(true == _object["hello"].is<int>());
    REQUIRE(false == _object["hello"].is<double>());
  }

  SECTION("StoreDouble") {
    _object["hello"] = 123.45;

    REQUIRE(true == _object["hello"].is<double>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(123.45 == _object["hello"].as<double>());
  }

  SECTION("StoreDoubleWithDigits") {
    _object["hello"].set(123.45, 2);

    REQUIRE(true == _object["hello"].is<double>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(123.45 == _object["hello"].as<double>());
  }

  SECTION("StoreBoolean") {
    _object["hello"] = true;

    REQUIRE(true == _object["hello"].is<bool>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(true == _object["hello"].as<bool>());
  }

  SECTION("StoreString") {
    _object["hello"] = "h3110";

    REQUIRE(true == _object["hello"].is<const char*>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(std::string("h3110") == _object["hello"].as<const char*>());
    REQUIRE(std::string("h3110") ==
            _object["hello"].as<char*>());  // <- short hand
  }

  SECTION("StoreArray") {
    JsonArray& arr = _jsonBuffer.createArray();

    _object["hello"] = arr;

    REQUIRE(&arr == &_object["hello"].as<JsonArray&>());
    REQUIRE(&arr == &_object["hello"].as<JsonArray>());  // <- short hand
    REQUIRE(&arr == &_object["hello"].as<const JsonArray&>());
    REQUIRE(&arr == &_object["hello"].as<const JsonArray>());  // <- short hand
    REQUIRE(true == _object["hello"].is<JsonArray&>());
    REQUIRE(true == _object["hello"].is<JsonArray>());
    REQUIRE(true == _object["hello"].is<const JsonArray&>());
    REQUIRE(true == _object["hello"].is<const JsonArray>());
    REQUIRE(false == _object["hello"].is<JsonObject&>());
  }

  SECTION("StoreObject") {
    JsonObject& obj = _jsonBuffer.createObject();

    _object["hello"] = obj;

    REQUIRE(&obj == &_object["hello"].as<JsonObject&>());
    REQUIRE(&obj == &_object["hello"].as<JsonObject>());  // <- short hand
    REQUIRE(&obj == &_object["hello"].as<const JsonObject&>());
    REQUIRE(&obj == &_object["hello"].as<const JsonObject>());  // <- short hand
    REQUIRE(true == _object["hello"].is<JsonObject&>());
    REQUIRE(true == _object["hello"].is<JsonObject>());
    REQUIRE(true == _object["hello"].is<const JsonObject&>());
    REQUIRE(true == _object["hello"].is<const JsonObject>());
    REQUIRE(false == _object["hello"].is<JsonArray&>());
  }

  SECTION("StoreArraySubscript") {
    JsonArray& arr = _jsonBuffer.createArray();
    arr.add(42);

    _object["a"] = arr[0];

    REQUIRE(42 == _object["a"]);
  }

  SECTION("StoreObjectSubscript") {
    JsonObject& obj = _jsonBuffer.createObject();
    obj.set("x", 42);

    _object["a"] = obj["x"];

    REQUIRE(42 == _object["a"]);
  }

  SECTION("KeyAsCharArray") {  // issue #423
    char key[] = "hello";
    _object[key] = 42;
    REQUIRE(42 == _object[key]);
  }
}
