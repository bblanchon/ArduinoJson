// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("StaticJsonBuffer::createObject()") {
  SECTION("GrowsWithObject") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(3)> buffer;

    JsonObject &obj = buffer.createObject();
    REQUIRE(JSON_OBJECT_SIZE(0) == buffer.size());

    obj["hello"];
    REQUIRE(JSON_OBJECT_SIZE(0) == buffer.size());

    obj["hello"] = 1;
    REQUIRE(JSON_OBJECT_SIZE(1) == buffer.size());

    obj["world"] = 2;
    REQUIRE(JSON_OBJECT_SIZE(2) == buffer.size());

    obj["world"] = 3;  // <- same key, should not grow
    REQUIRE(JSON_OBJECT_SIZE(2) == buffer.size());
  }

  SECTION("SucceedWhenBigEnough") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(0)> buffer;

    JsonObject &object = buffer.createObject();
    REQUIRE(object.success());
  }

  SECTION("FailsWhenTooSmall") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(0) - 1> buffer;

    JsonObject &object = buffer.createObject();
    REQUIRE_FALSE(object.success());
  }

  SECTION("ObjectDoesntGrowWhenFull") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1)> buffer;

    JsonObject &obj = buffer.createObject();
    obj["hello"] = 1;
    obj["world"] = 2;

    REQUIRE(JSON_OBJECT_SIZE(1) == buffer.size());
    REQUIRE(1 == obj.size());

    char json[64];
    obj.printTo(json, sizeof(json));
    REQUIRE(std::string("{\"hello\":1}") == json);
  }
}
