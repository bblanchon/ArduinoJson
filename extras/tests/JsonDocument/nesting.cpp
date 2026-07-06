// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonDocument::nesting()") {
  JsonDocument doc;

  SUBCASE("return 0 if uninitialized") {
    REQUIRE(doc.nesting() == 0);
  }

  SUBCASE("returns 0 for string") {
    JsonVariant var = doc.to<JsonVariant>();
    var.set("hello");
    REQUIRE(doc.nesting() == 0);
  }

  SUBCASE("returns 1 for empty object") {
    doc.to<JsonObject>();
    REQUIRE(doc.nesting() == 1);
  }

  SUBCASE("returns 1 for empty array") {
    doc.to<JsonArray>();
    REQUIRE(doc.nesting() == 1);
  }
}
