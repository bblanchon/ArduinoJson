// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonVariantConst::containsKey()") {
  JsonDocument doc;
  doc["hello"] = "world";
  JsonVariantConst var = doc.as<JsonVariant>();

  SECTION("support const char*") {
    REQUIRE(var.containsKey("hello") == true);
    REQUIRE(var.containsKey("world") == false);
  }

  SECTION("support std::string") {
    REQUIRE(var.containsKey("hello"_s) == true);
    REQUIRE(var.containsKey("world"_s) == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == var.containsKey(vla));
  }
#endif

  SECTION("support JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(var.containsKey(var["key"]) == true);
    REQUIRE(var.containsKey(var["foo"]) == false);
  }
}
