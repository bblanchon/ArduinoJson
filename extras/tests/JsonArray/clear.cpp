// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::clear()") {
  SECTION("No-op on null JsonArray") {
    JsonArray array;
    array.clear();
    REQUIRE(array.isNull() == true);
    REQUIRE(array.size() == 0);
  }

  SECTION("Removes all elements") {
    StaticJsonDocument<64> doc;
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add(2);
    array.clear();
    REQUIRE(array.size() == 0);
    REQUIRE(array.isNull() == false);
  }
}
