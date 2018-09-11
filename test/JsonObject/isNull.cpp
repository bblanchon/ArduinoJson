// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::isNull()") {
  SECTION("returns true for undefined JsonObject") {
    JsonObject array;
    REQUIRE(array.isNull() == true);
  }

  SECTION("returns false when allocation succeeds") {
    StaticJsonDocument<JSON_OBJECT_SIZE(0)> doc;
    JsonObject array = doc.to<JsonObject>();
    REQUIRE(array.isNull() == false);
  }

  /*  SECTION("returns true when allocation fails") {
      StaticJsonDocument<1> doc;
      JsonObject array = doc.to<JsonObject>();
      REQUIRE(array.isNull() == true);
    }*/
}
