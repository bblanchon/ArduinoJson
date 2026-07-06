// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <stdint.h>

TEST_CASE("JsonArrayConst::operator[]") {
  JsonDocument doc;
  JsonArrayConst arr = doc.to<JsonArray>();
  doc.add(1);
  doc.add(2);
  doc.add(3);

  SUBCASE("int") {
    REQUIRE(1 == arr[0].as<int>());
    REQUIRE(2 == arr[1].as<int>());
    REQUIRE(3 == arr[2].as<int>());
    REQUIRE(0 == arr[3].as<int>());
  }

  SUBCASE("JsonVariant") {
    REQUIRE(2 == arr[arr[0]].as<int>());
    REQUIRE(0 == arr[arr[3]].as<int>());
  }
}
