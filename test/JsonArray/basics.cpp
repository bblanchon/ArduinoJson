// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray basics") {
  DynamicJsonBuffer jb;
  JsonArray& array = jb.createArray();

  SECTION("SuccessIsTrue") {
    REQUIRE(array.success());
  }

  SECTION("InitialSizeIsZero") {
    REQUIRE(0U == array.size());
  }

  SECTION("CreateNestedArray") {
    JsonArray& arr = array.createNestedArray();
    REQUIRE(&arr == &array[0].as<JsonArray&>());
  }

  SECTION("CreateNestedObject") {
    JsonObject& obj = array.createNestedObject();
    REQUIRE(&obj == &array[0].as<JsonObject&>());
  }
}
