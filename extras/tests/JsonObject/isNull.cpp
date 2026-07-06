// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::isNull()") {
  SECTION("returns true") {
    JsonObject obj;
    REQUIRE(obj.isNull() == true);
  }

  SECTION("returns false") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    REQUIRE(obj.isNull() == false);
  }
}

TEST_CASE("JsonObject::operator bool()") {
  SECTION("returns false") {
    JsonObject obj;
    REQUIRE(static_cast<bool>(obj) == false);
  }

  SECTION("returns true") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    REQUIRE(static_cast<bool>(obj) == true);
  }
}
