// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::memoryUsage()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();

  SECTION("return 0 if uninitialized") {
    JsonObject unitialized;
    REQUIRE(unitialized.memoryUsage() == 0);
  }

  SECTION("JSON_OBJECT_SIZE(0) for empty object") {
    REQUIRE(obj.memoryUsage() == JSON_OBJECT_SIZE(0));
  }

  SECTION("JSON_OBJECT_SIZE(1) after add") {
    obj["hello"] = 42;
    REQUIRE(obj.memoryUsage() == JSON_OBJECT_SIZE(1));
  }

  SECTION("includes the size of the key") {
    obj[std::string("hello")] = 42;
    REQUIRE(obj.memoryUsage() == JSON_OBJECT_SIZE(1) + 6);
  }

  SECTION("includes the size of the nested array") {
    JsonArray nested = obj.createNestedArray("nested");
    nested.add(42);
    REQUIRE(obj.memoryUsage() == JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(1));
  }

  SECTION("includes the size of the nested object") {
    JsonObject nested = obj.createNestedObject("nested");
    nested["hello"] = "world";
    REQUIRE(obj.memoryUsage() == 2 * JSON_OBJECT_SIZE(1));
  }
}
