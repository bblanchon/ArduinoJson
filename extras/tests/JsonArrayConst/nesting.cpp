// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonArrayConst::nesting()") {
  JsonDocument doc;
  JsonArrayConst arr = doc.to<JsonArray>();

  SUBCASE("return 0 if unbound") {
    JsonArrayConst unbound;
    REQUIRE(unbound.nesting() == 0);
  }

  SUBCASE("returns 1 for empty array") {
    REQUIRE(arr.nesting() == 1);
  }

  SUBCASE("returns 1 for flat array") {
    doc.add("hello");
    REQUIRE(arr.nesting() == 1);
  }

  SUBCASE("returns 2 with nested array") {
    doc.add<JsonArray>();
    REQUIRE(arr.nesting() == 2);
  }

  SUBCASE("returns 2 with nested object") {
    doc.add<JsonObject>();
    REQUIRE(arr.nesting() == 2);
  }
}
