// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::get()") {
  DynamicJsonDocument doc(4096);
  deserializeJson(doc, "[1,2,3]");
  JsonArray array = doc.as<JsonArray>();

  SECTION("Overflow") {
    REQUIRE(array.getElement(3).isNull());
  }
}
