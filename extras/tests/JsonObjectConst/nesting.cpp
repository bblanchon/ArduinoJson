// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObjectConst::nesting()") {
  JsonDocument doc;
  JsonObjectConst obj = doc.to<JsonObject>();

  SUBCASE("return 0 if unbound") {
    JsonObjectConst unbound;
    REQUIRE(unbound.nesting() == 0);
  }

  SUBCASE("returns 1 for empty object") {
    REQUIRE(obj.nesting() == 1);
  }

  SUBCASE("returns 1 for flat object") {
    doc["hello"] = "world";
    REQUIRE(obj.nesting() == 1);
  }

  SUBCASE("returns 2 with nested array") {
    doc["nested"].to<JsonArray>();
    REQUIRE(obj.nesting() == 2);
  }

  SUBCASE("returns 2 with nested object") {
    doc["nested"].to<JsonObject>();
    REQUIRE(obj.nesting() == 2);
  }
}
