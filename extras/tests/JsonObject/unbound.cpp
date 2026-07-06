// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("Unbound JsonObject") {
  JsonObject obj;

  SECTION("retrieve member") {
    REQUIRE(obj["key"].isNull());
  }

  SECTION("add member") {
    obj["hello"] = "world";
    REQUIRE(0 == obj.size());
  }

  SECTION("serialize") {
    char buffer[32];
    serializeJson(obj, buffer, sizeof(buffer));
    REQUIRE_THAT(buffer, Equals("null"));
  }
}
