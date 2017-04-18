// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject basics") {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  SECTION("InitialSizeIsZero") {
    REQUIRE(0 == _object.size());
  }

  SECTION("SuccessIsTrue") {
    REQUIRE(_object.success());
  }
}
