// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::isNull()") {
  SECTION("returns true for undefined JsonArray") {
    JsonArray array;
    REQUIRE(array.isNull() == true);
  }

  SECTION("returns false when allocation succeeds") {
    StaticJsonDocument<JSON_ARRAY_SIZE(0)> doc;
    JsonArray array = doc.to<JsonArray>();
    REQUIRE(array.isNull() == false);
  }

  /*  SECTION("returns true when allocation fails") {
      StaticJsonDocument<1> doc;
      JsonArray array = doc.to<JsonArray>();
      REQUIRE(array.isNull() == true);
    }*/
}
