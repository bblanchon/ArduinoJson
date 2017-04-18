// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::copyTo()") {
  DynamicJsonBuffer jsonBuffer;

  SECTION("BiggerOneDimensionIntegerArray") {
    char json[] = "[1,2,3]";
    JsonArray& array = jsonBuffer.parseArray(json);

    int destination[4] = {0};
    size_t result = array.copyTo(destination);

    REQUIRE(3 == result);
    REQUIRE(1 == destination[0]);
    REQUIRE(2 == destination[1]);
    REQUIRE(3 == destination[2]);
    REQUIRE(0 == destination[3]);
  }

  SECTION("SmallerOneDimensionIntegerArray") {
    char json[] = "[1,2,3]";
    JsonArray& array = jsonBuffer.parseArray(json);

    int destination[2] = {0};
    size_t result = array.copyTo(destination);

    REQUIRE(2 == result);
    REQUIRE(1 == destination[0]);
    REQUIRE(2 == destination[1]);
  }

  SECTION("TwoOneDimensionIntegerArray") {
    char json[] = "[[1,2],[3],[4]]";

    JsonArray& array = jsonBuffer.parseArray(json);

    int destination[3][2] = {{0}};
    array.copyTo(destination);

    REQUIRE(1 == destination[0][0]);
    REQUIRE(2 == destination[0][1]);
    REQUIRE(3 == destination[1][0]);
    REQUIRE(0 == destination[1][1]);
    REQUIRE(4 == destination[2][0]);
    REQUIRE(0 == destination[2][1]);
  }
}
