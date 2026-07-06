// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonObjectConst::isNull()") {
  SUBCASE("returns true") {
    JsonObjectConst obj;
    REQUIRE(obj.isNull() == true);
  }

  SUBCASE("returns false") {
    JsonDocument doc;
    JsonObjectConst obj = doc.to<JsonObject>();
    REQUIRE(obj.isNull() == false);
  }
}

TEST_CASE("JsonObjectConst::operator bool()") {
  SUBCASE("returns false") {
    JsonObjectConst obj;
    REQUIRE(static_cast<bool>(obj) == false);
  }

  SUBCASE("returns true") {
    JsonDocument doc;
    JsonObjectConst obj = doc.to<JsonObject>();
    REQUIRE(static_cast<bool>(obj) == true);
  }
}
