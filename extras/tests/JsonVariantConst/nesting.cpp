// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariantConst::nesting()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  SECTION("return 0 if unbound") {
    JsonVariantConst unbound;
    REQUIRE(unbound.nesting() == 0);
  }

  SECTION("returns 0 for string") {
    doc.set("hello");
    REQUIRE(var.nesting() == 0);
  }

  SECTION("returns 1 for empty object") {
    doc.to<JsonObject>();
    REQUIRE(var.nesting() == 1);
  }

  SECTION("returns 1 for empty array") {
    doc.to<JsonArray>();
    REQUIRE(var.nesting() == 1);
  }
}
