// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::get()") {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("get(const char*)") {
    var["value"] = 42;

    REQUIRE(var.get("value") == 42);
  }

  SECTION("get(std::string)") {
    var["value"] = 42;

    REQUIRE(var.get(std::string("value")) == 42);
  }

  SECTION("get(int)") {
    var.add().set(42);

    REQUIRE(var.get(0) == 42);
  }
}
