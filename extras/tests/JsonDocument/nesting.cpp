// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::nesting()") {
  DynamicJsonDocument doc(4096);

  SECTION("return 0 if uninitialized") {
    REQUIRE(doc.nesting() == 0);
  }

  SECTION("returns 0 for string") {
    JsonVariant var = doc.to<JsonVariant>();
    var.set("hello");
    REQUIRE(doc.nesting() == 0);
  }

  SECTION("returns 1 for empty object") {
    doc.to<JsonObject>();
    REQUIRE(doc.nesting() == 1);
  }

  SECTION("returns 1 for empty array") {
    doc.to<JsonArray>();
    REQUIRE(doc.nesting() == 1);
  }
}
