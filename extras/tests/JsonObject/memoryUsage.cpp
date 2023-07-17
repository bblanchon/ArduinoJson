// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;
using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonObject::memoryUsage()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("return 0 if uninitialized") {
    JsonObject unitialized;
    REQUIRE(unitialized.memoryUsage() == 0);
  }

  SECTION("sizeofObject(0) for empty object") {
    REQUIRE(obj.memoryUsage() == sizeofObject(0));
  }

  SECTION("sizeofObject(1) after add") {
    obj["hello"] = 42;
    REQUIRE(obj.memoryUsage() == sizeofObject(1));
  }

  SECTION("includes the size of the key") {
    obj[std::string("hello")] = 42;
    REQUIRE(obj.memoryUsage() == sizeofObject(1) + sizeofString(5));
  }

  SECTION("includes the size of the nested array") {
    JsonArray nested = obj.createNestedArray("nested");
    nested.add(42);
    REQUIRE(obj.memoryUsage() == sizeofObject(1) + sizeofArray(1));
  }

  SECTION("includes the size of the nested object") {
    JsonObject nested = obj.createNestedObject("nested");
    nested["hello"] = "world";
    REQUIRE(obj.memoryUsage() == 2 * sizeofObject(1));
  }
}
