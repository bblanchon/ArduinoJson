// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObject::nesting()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SUBCASE("return 0 if uninitialized") {
    JsonObject unitialized;
    REQUIRE(unitialized.nesting() == 0);
  }

  SUBCASE("returns 1 for empty object") {
    REQUIRE(obj.nesting() == 1);
  }

  SUBCASE("returns 1 for flat object") {
    obj["hello"] = "world";
    REQUIRE(obj.nesting() == 1);
  }

  SUBCASE("returns 2 with nested array") {
    obj["nested"].to<JsonArray>();
    REQUIRE(obj.nesting() == 2);
  }

  SUBCASE("returns 2 with nested object") {
    obj["nested"].to<JsonObject>();
    REQUIRE(obj.nesting() == 2);
  }
}
