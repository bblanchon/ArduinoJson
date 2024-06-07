// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonVariant::containsKey()") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("containsKey(const char*)") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SECTION("containsKey(std::string)") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello"_s) == true);
    REQUIRE(var.containsKey("world"_s) == false);
  }

  SECTION("containsKey(JsonVariant)") {
    var["hello"] = "world";
    var["key"] = "hello";

    REQUIRE(var.containsKey(doc["key"]) == true);
    REQUIRE(var.containsKey(doc["foo"]) == false);
  }
}
