// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::invalid()") {
  JsonObject& obj = JsonObject::invalid();

  SECTION("SubscriptFails") {
    REQUIRE_FALSE(obj["key"].success());
  }

  SECTION("AddFails") {
    obj.set("hello", "world");
    REQUIRE(0 == obj.size());
  }

  SECTION("CreateNestedArrayFails") {
    REQUIRE_FALSE(obj.createNestedArray("hello").success());
  }

  SECTION("CreateNestedObjectFails") {
    REQUIRE_FALSE(obj.createNestedObject("world").success());
  }

  SECTION("PrintToWritesBraces") {
    char buffer[32];
    obj.printTo(buffer, sizeof(buffer));
    REQUIRE_THAT(buffer, Equals("{}"));
  }
}
