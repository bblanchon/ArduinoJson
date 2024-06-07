// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonVariantConst::as<T>()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  doc.set("hello");

  REQUIRE(var.as<bool>() == true);
  REQUIRE(var.as<long>() == 0L);
  REQUIRE(var.as<const char*>() == "hello"_s);
  REQUIRE(var.as<std::string>() == "hello"_s);
}

TEST_CASE("Invalid conversions") {
  using namespace ArduinoJson::detail;

  JsonVariantConst variant;

  CHECK(is_same<decltype(variant.as<int>()), int>::value);
  CHECK(is_same<decltype(variant.as<float>()), float>::value);
  CHECK(is_same<decltype(variant.as<JsonVariantConst>()),
                JsonVariantConst>::value);
  CHECK(
      is_same<decltype(variant.as<JsonObjectConst>()), JsonObjectConst>::value);
  CHECK(is_same<decltype(variant.as<JsonArrayConst>()), JsonArrayConst>::value);

  CHECK(is_same<decltype(variant.as<JsonVariant>()),
                InvalidConversion<JsonVariantConst, JsonVariant>>::value);
  CHECK(is_same<decltype(variant.as<JsonObject>()),
                InvalidConversion<JsonVariantConst, JsonObject>>::value);
  CHECK(is_same<decltype(variant.as<JsonArray>()),
                InvalidConversion<JsonVariantConst, JsonArray>>::value);
}
