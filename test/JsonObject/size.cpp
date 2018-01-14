// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::size()") {
  DynamicJsonBuffer jb;
  JsonObject& _object = jb.createObject();

  SECTION("increases when values are added") {
    _object.set("hello", 42);
    REQUIRE(1 == _object.size());
  }

  SECTION("doesn't increase when the smae key is added twice") {
    _object["hello"] = 1;
    _object["hello"] = 2;
    REQUIRE(1 == _object.size());
  }
}
