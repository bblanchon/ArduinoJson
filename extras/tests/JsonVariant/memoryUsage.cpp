// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

TEST_CASE("JsonVariant::memoryUsage()") {
  JsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("returns 0 if uninitialized") {
    JsonVariant unitialized;
    REQUIRE(unitialized.memoryUsage() == 0);
  }

  SECTION("returns size of object") {
    JsonObject obj = var.to<JsonObject>();
    obj["hello"] = 42;
    REQUIRE(var.memoryUsage() == sizeofObject(1));
  }

  SECTION("returns size of array") {
    JsonArray arr = var.to<JsonArray>();
    arr.add(42);
    REQUIRE(var.memoryUsage() == sizeofArray(1));
  }

  SECTION("returns size of owned string") {
    var.set(std::string("hello"));
    REQUIRE(var.memoryUsage() == 6);
    REQUIRE(var.memoryUsage() == doc.memoryUsage());
  }

  SECTION("returns size of owned raw") {
    var.set(serialized(std::string("hello")));
    REQUIRE(var.memoryUsage() == 6);
    REQUIRE(var.memoryUsage() == doc.memoryUsage());
  }
}
