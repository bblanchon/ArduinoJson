// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::invalid()") {
  JsonObject obj;

  SECTION("SubscriptFails") {
    REQUIRE(obj["key"].isNull());
  }

  SECTION("AddFails") {
    obj["hello"] = "world";
    REQUIRE(0 == obj.size());
  }

  SECTION("CreateNestedArrayFails") {
    REQUIRE(obj.createNestedArray("hello").isNull());
  }

  SECTION("CreateNestedObjectFails") {
    REQUIRE(obj.createNestedObject("world").isNull());
  }

  SECTION("serialize to 'null'") {
    char buffer[32];
    serializeJson(obj, buffer, sizeof(buffer));
    REQUIRE_THAT(buffer, Equals("null"));
  }
}
