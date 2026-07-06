// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObject::isNull()") {
  SUBCASE("returns true") {
    JsonObject obj;
    REQUIRE(obj.isNull() == true);
  }

  SUBCASE("returns false") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    REQUIRE(obj.isNull() == false);
  }
}

TEST_CASE("JsonObject::operator bool()") {
  SUBCASE("returns false") {
    JsonObject obj;
    REQUIRE(static_cast<bool>(obj) == false);
  }

  SUBCASE("returns true") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    REQUIRE(static_cast<bool>(obj) == true);
  }
}
