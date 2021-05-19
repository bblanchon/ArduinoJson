// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

static const char* null = 0;

TEST_CASE("JsonVariant::containsKey()") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("containsKey(const char*) returns true") {
    var["hello"] = "world";

    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SECTION("containsKey(std::string) returns true") {
    var["hello"] = "world";

    REQUIRE(var.containsKey(std::string("hello")) == true);
    REQUIRE(var.containsKey(std::string("world")) == false);
  }
}

TEST_CASE("JsonVariantConst::containsKey()") {
  DynamicJsonDocument doc(4096);
  doc["hello"] = "world";
  JsonVariantConst cvar = doc.as<JsonVariant>();

  SECTION("containsKey(const char*) returns true") {
    REQUIRE(cvar.containsKey("hello") == true);
    REQUIRE(cvar.containsKey("world") == false);
  }

  SECTION("containsKey(std::string) returns true") {
    REQUIRE(cvar.containsKey(std::string("hello")) == true);
    REQUIRE(cvar.containsKey(std::string("world")) == false);
  }
}
