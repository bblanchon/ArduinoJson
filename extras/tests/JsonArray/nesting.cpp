// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonArray::nesting()") {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  SUBCASE("return 0 if uninitialized") {
    JsonArray unitialized;
    REQUIRE(unitialized.nesting() == 0);
  }

  SUBCASE("returns 1 for empty array") {
    REQUIRE(arr.nesting() == 1);
  }

  SUBCASE("returns 1 for flat array") {
    arr.add("hello");
    REQUIRE(arr.nesting() == 1);
  }

  SUBCASE("returns 2 with nested array") {
    arr.add<JsonArray>();
    REQUIRE(arr.nesting() == 2);
  }

  SUBCASE("returns 2 with nested object") {
    arr.add<JsonObject>();
    REQUIRE(arr.nesting() == 2);
  }
}
