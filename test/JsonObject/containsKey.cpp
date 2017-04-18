// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::containsKey()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  SECTION("ContainsKeyReturnsFalseForNonExistingKey") {
    _object.set("hello", 42);

    REQUIRE(false == _object.containsKey("world"));
  }

  SECTION("ContainsKeyReturnsTrueForDefinedValue") {
    _object.set("hello", 42);

    REQUIRE(true == _object.containsKey("hello"));
  }

  SECTION("ContainsKeyReturnsFalseAfterRemove") {
    _object.set("hello", 42);
    _object.remove("hello");

    REQUIRE(false == _object.containsKey("hello"));
  }
}
