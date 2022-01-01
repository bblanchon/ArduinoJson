// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::isNull()") {
  DynamicJsonDocument doc(4096);

  SECTION("returns true if uninitialized") {
    REQUIRE(doc.isNull() == true);
  }

  SECTION("returns false after to<JsonObject>()") {
    doc.to<JsonObject>();
    REQUIRE(doc.isNull() == false);
  }

  SECTION("returns false after to<JsonArray>()") {
    doc.to<JsonArray>();
    REQUIRE(doc.isNull() == false);
  }

  SECTION("returns true after to<JsonVariant>()") {
    REQUIRE(doc.isNull() == true);
  }

  SECTION("returns false after set()") {
    doc.to<JsonVariant>().set(42);
    REQUIRE(doc.isNull() == false);
  }

  SECTION("returns true after clear()") {
    doc.to<JsonObject>();
    doc.clear();
    REQUIRE(doc.isNull() == true);
  }
}
