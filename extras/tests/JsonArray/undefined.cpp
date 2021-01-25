// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("Undefined JsonArray") {
  JsonArray array;

  SECTION("SubscriptFails") {
    REQUIRE(array[0].isNull());
  }

  SECTION("AddFails") {
    array.add(1);
    REQUIRE(0 == array.size());
  }

  SECTION("CreateNestedArrayFails") {
    REQUIRE(array.createNestedArray().isNull());
  }

  SECTION("CreateNestedObjectFails") {
    REQUIRE(array.createNestedObject().isNull());
  }

  SECTION("PrintToWritesBrackets") {
    char buffer[32];
    serializeJson(array, buffer, sizeof(buffer));
    REQUIRE_THAT(buffer, Equals("null"));
  }
}
