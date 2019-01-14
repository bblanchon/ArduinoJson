// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::get()") {
  DynamicJsonDocument doc(4096);
  deserializeJson(doc, "[1,2,3]");
  JsonArray array = doc.as<JsonArray>();

  SECTION("Overflow") {
    REQUIRE(array.get(3).isNull());
  }
}
