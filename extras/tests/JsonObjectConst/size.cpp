// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <string>

TEST_CASE("JsonObjectConst::size()") {
  JsonDocument doc;
  JsonObjectConst obj = doc.to<JsonObject>();

  SUBCASE("returns 0 when empty") {
    REQUIRE(0 == obj.size());
  }

  SUBCASE("returns the number of members") {
    doc["hello"] = 1;
    doc["world"] = 2;
    REQUIRE(2 == obj.size());
  }
}
