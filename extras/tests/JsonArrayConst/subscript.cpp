// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonArrayConst::operator[]") {
  JsonDocument doc;
  JsonArrayConst arr = doc.to<JsonArray>();
  doc.add(1);
  doc.add(2);
  doc.add(3);

  REQUIRE(1 == arr[0].as<int>());
  REQUIRE(2 == arr[1].as<int>());
  REQUIRE(3 == arr[2].as<int>());
  REQUIRE(0 == arr[3].as<int>());
}
