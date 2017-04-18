// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonBuffer::createObject()") {
  DynamicJsonBuffer json;

  JsonObject &obj = json.createObject();
  REQUIRE(JSON_OBJECT_SIZE(0) == json.size());

  obj["hello"] = 1;
  REQUIRE(JSON_OBJECT_SIZE(1) == json.size());

  obj["world"] = 2;
  REQUIRE(JSON_OBJECT_SIZE(2) == json.size());

  obj["world"] = 3;  // <- same key, should not grow
  REQUIRE(JSON_OBJECT_SIZE(2) == json.size());
}
