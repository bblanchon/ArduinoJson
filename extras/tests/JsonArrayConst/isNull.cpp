// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonArrayConst::isNull()") {
  SUBCASE("returns true") {
    JsonArrayConst arr;
    REQUIRE(arr.isNull() == true);
  }

  SUBCASE("returns false") {
    JsonDocument doc;
    JsonArrayConst arr = doc.to<JsonArray>();
    REQUIRE(arr.isNull() == false);
  }
}

TEST_CASE("JsonArrayConst::operator bool()") {
  SUBCASE("returns false") {
    JsonArrayConst arr;
    REQUIRE(static_cast<bool>(arr) == false);
  }

  SUBCASE("returns true") {
    JsonDocument doc;
    JsonArrayConst arr = doc.to<JsonArray>();
    REQUIRE(static_cast<bool>(arr) == true);
  }
}
