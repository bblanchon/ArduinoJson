// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <stdint.h>

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

  CHECK((is_same<decltype(variant.as<int>()), int>::value == true));
  CHECK((is_same<decltype(variant.as<float>()), float>::value == true));
  CHECK((is_same<decltype(variant.as<JsonVariantConst>()),
                 JsonVariantConst>::value == true));
  CHECK((is_same<decltype(variant.as<JsonObjectConst>()),
                 JsonObjectConst>::value == true));
  CHECK(
      (is_same<decltype(variant.as<JsonArrayConst>()), JsonArrayConst>::value ==
       true));

  CHECK((is_same<decltype(variant.as<JsonVariant>()),
                 InvalidConversion<JsonVariantConst, JsonVariant>>::value ==
         true));
  CHECK((is_same<decltype(variant.as<JsonObject>()),
                 InvalidConversion<JsonVariantConst, JsonObject>>::value ==
         true));
  CHECK(
      (is_same<decltype(variant.as<JsonArray>()),
               InvalidConversion<JsonVariantConst, JsonArray>>::value == true));
}
