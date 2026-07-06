// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonArray::isNull()") {
  SUBCASE("returns true") {
    JsonArray arr;
    REQUIRE(arr.isNull() == true);
  }

  SUBCASE("returns false") {
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    REQUIRE(arr.isNull() == false);
  }
}

TEST_CASE("JsonArray::operator bool()") {
  SUBCASE("returns false") {
    JsonArray arr;
    REQUIRE(static_cast<bool>(arr) == false);
  }

  SUBCASE("returns true") {
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    REQUIRE(static_cast<bool>(arr) == true);
  }
}
