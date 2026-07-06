// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonDocument::size()") {
  JsonDocument doc;

  SUBCASE("returns 0") {
    REQUIRE(doc.size() == 0);
  }

  SUBCASE("as an array, return 2") {
    doc.add(1);
    doc.add(2);

    REQUIRE(doc.size() == 2);
  }

  SUBCASE("as an object, return 2") {
    doc["a"] = 1;
    doc["b"] = 2;

    REQUIRE(doc.size() == 2);
  }
}
