// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::add()") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("integer") {
    var.add(42);

    REQUIRE(var.as<std::string>() == "[42]");
  }

  SECTION("const char*") {
    var.add("hello");

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }

  SECTION("std::string") {
    var.add(std::string("hello"));

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }
}
