// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonArrayConst::size()") {
  JsonDocument doc;
  JsonArrayConst array = doc.to<JsonArray>();

  SUBCASE("returns 0 if unbound") {
    JsonArrayConst unbound;
    REQUIRE(0U == unbound.size());
  }

  SUBCASE("returns 0 is empty") {
    REQUIRE(0U == array.size());
  }

  SUBCASE("return number of elements") {
    doc.add("hello");
    doc.add("world");

    REQUIRE(2U == array.size());
  }
}
