// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObjectConst::size()") {
  JsonDocument doc;
  JsonObjectConst obj = doc.to<JsonObject>();

  SECTION("returns 0 when empty") {
    REQUIRE(0 == obj.size());
  }

  SECTION("returns the number of members") {
    doc["hello"] = 1;
    doc["world"] = 2;
    REQUIRE(2 == obj.size());
  }
}
