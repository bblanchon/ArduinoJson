// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("StaticJsonBuffer::createArray()") {
  SECTION("GrowsWithArray") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(2)> json;

    JsonArray &array = json.createArray();
    REQUIRE(JSON_ARRAY_SIZE(0) == json.size());

    array.add("hello");
    REQUIRE(JSON_ARRAY_SIZE(1) == json.size());

    array.add("world");
    REQUIRE(JSON_ARRAY_SIZE(2) == json.size());
  }

  SECTION("SucceedWhenBigEnough") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(0)> json;

    JsonArray &array = json.createArray();
    REQUIRE(array.success());
  }

  SECTION("FailsWhenTooSmall") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(0) - 1> json;

    JsonArray &array = json.createArray();
    REQUIRE_FALSE(array.success());
  }

  SECTION("ArrayDoesntGrowWhenFull") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(1)> json;

    JsonArray &array = json.createArray();
    array.add("hello");
    array.add("world");

    REQUIRE(1 == array.size());
  }
}
