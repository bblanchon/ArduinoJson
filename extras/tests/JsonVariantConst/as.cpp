// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonVariantConst::as<T>()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  doc.set("hello");

  REQUIRE(var.as<bool>() == true);
  REQUIRE(var.as<long>() == 0L);
  REQUIRE(var.as<const char*>() == std::string("hello"));
  REQUIRE(var.as<std::string>() == std::string("hello"));
}
