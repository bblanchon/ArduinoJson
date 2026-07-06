// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Literals.hpp"

TEST_CASE("Unbound JsonObject") {
  JsonObject obj;

  SUBCASE("retrieve member") {
    REQUIRE(obj["key"].isNull());
  }

  SUBCASE("add member") {
    obj["hello"] = "world";
    REQUIRE(0 == obj.size());
  }

  SUBCASE("serialize") {
    char buffer[32];
    serializeJson(obj, buffer, sizeof(buffer));
    REQUIRE(buffer == "null"_s);
  }
}
