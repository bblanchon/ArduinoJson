// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("Unbound JsonArray") {
  JsonArray array;

  SECTION("SubscriptFails") {
    REQUIRE(array[0].isNull());
  }

  SECTION("AddFails") {
    array.add(1);
    REQUIRE(0 == array.size());
  }

  SECTION("PrintToWritesBrackets") {
    char buffer[32];
    serializeJson(array, buffer, sizeof(buffer));
    REQUIRE_THAT(buffer, Equals("null"));
  }
}
