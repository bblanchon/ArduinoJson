// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::memoryUsage()") {
  DynamicJsonDocument doc(4096);
  JsonArray arr = doc.to<JsonArray>();

  SECTION("return 0 if uninitialized") {
    JsonArray unitialized;
    REQUIRE(unitialized.memoryUsage() == 0);
  }

  SECTION("JSON_ARRAY_SIZE(0) if empty") {
    REQUIRE(arr.memoryUsage() == JSON_ARRAY_SIZE(0));
  }

  SECTION("JSON_ARRAY_SIZE(1) after add") {
    arr.add("hello");
    REQUIRE(arr.memoryUsage() == JSON_ARRAY_SIZE(1));
  }

  SECTION("includes the size of the string") {
    arr.add(std::string("hello"));
    REQUIRE(arr.memoryUsage() == JSON_ARRAY_SIZE(1) + 6);
  }

  SECTION("includes the size of the nested array") {
    JsonArray nested = arr.createNestedArray();
    nested.add(42);
    REQUIRE(arr.memoryUsage() == 2 * JSON_ARRAY_SIZE(1));
  }

  SECTION("includes the size of the nested arrect") {
    JsonObject nested = arr.createNestedObject();
    nested["hello"] = "world";
    REQUIRE(arr.memoryUsage() == JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(1));
  }
}
