// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject basics") {
  DynamicJsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("isNull()") {
    REQUIRE(obj.isNull() == false);
  }
}
