// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::size()") {
  DynamicJsonDocument doc;
  JsonObject& obj = doc.to<JsonObject>();

  SECTION("initial size is zero") {
    REQUIRE(0 == obj.size());
  }

  SECTION("increases when values are added") {
    obj.set("hello", 42);
    REQUIRE(1 == obj.size());
  }

  SECTION("doesn't increase when the smae key is added twice") {
    obj["hello"] = 1;
    obj["hello"] = 2;
    REQUIRE(1 == obj.size());
  }
}
