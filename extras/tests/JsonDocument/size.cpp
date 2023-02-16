// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::size()") {
  DynamicJsonDocument doc(4096);

  SECTION("returns 0") {
    REQUIRE(doc.size() == 0);
  }

  SECTION("as an array, return 2") {
    doc.add(1);
    doc.add(2);

    REQUIRE(doc.size() == 2);
  }

  SECTION("as an object, return 2") {
    doc["a"] = 1;
    doc["b"] = 2;

    REQUIRE(doc.size() == 2);
  }
}
