// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Literals.hpp"

TEST_CASE("Unbound JsonArray") {
  JsonArray array;

  SUBCASE("SubscriptFails") {
    REQUIRE(array[0].isNull());
  }

  SUBCASE("AddFails") {
    array.add(1);
    REQUIRE(0 == array.size());
  }

  SUBCASE("PrintToWritesBrackets") {
    char buffer[32];
    serializeJson(array, buffer, sizeof(buffer));
    REQUIRE(buffer == "null"_s);
  }
}
