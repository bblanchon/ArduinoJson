// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonDocument::isNull()") {
  JsonDocument doc;

  SUBCASE("returns true if uninitialized") {
    REQUIRE(doc.isNull() == true);
  }

  SUBCASE("returns false after to<JsonObject>()") {
    doc.to<JsonObject>();
    REQUIRE(doc.isNull() == false);
  }

  SUBCASE("returns false after to<JsonArray>()") {
    doc.to<JsonArray>();
    REQUIRE(doc.isNull() == false);
  }

  SUBCASE("returns true after to<JsonVariant>()") {
    REQUIRE(doc.isNull() == true);
  }

  SUBCASE("returns false after set()") {
    doc.to<JsonVariant>().set(42);
    REQUIRE(doc.isNull() == false);
  }

  SUBCASE("returns true after clear()") {
    doc.to<JsonObject>();
    doc.clear();
    REQUIRE(doc.isNull() == true);
  }
}
