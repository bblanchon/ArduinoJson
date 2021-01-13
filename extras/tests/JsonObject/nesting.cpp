// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::nesting()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();

  SECTION("return 0 if uninitialized") {
    JsonObject unitialized;
    REQUIRE(unitialized.nesting() == 0);
  }

  SECTION("returns 1 for empty object") {
    REQUIRE(obj.nesting() == 1);
  }

  SECTION("returns 1 for flat object") {
    obj["hello"] = "world";
    REQUIRE(obj.nesting() == 1);
  }

  SECTION("returns 2 with nested array") {
    obj.createNestedArray("nested");
    REQUIRE(obj.nesting() == 2);
  }

  SECTION("returns 2 with nested object") {
    obj.createNestedObject("nested");
    REQUIRE(obj.nesting() == 2);
  }
}
