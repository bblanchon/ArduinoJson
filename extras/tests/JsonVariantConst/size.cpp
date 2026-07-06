// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonVariantConst::size()") {
  JsonDocument doc;
  JsonVariantConst variant = doc.to<JsonVariant>();

  SUBCASE("unbound reference") {
    JsonVariantConst unbound;

    CHECK(unbound.size() == 0);
  }

  SUBCASE("int") {
    doc.set(42);

    CHECK(variant.size() == 0);
  }

  SUBCASE("string") {
    doc.set("hello");

    CHECK(variant.size() == 0);
  }

  SUBCASE("object") {
    doc["a"] = 1;
    doc["b"] = 2;

    CHECK(variant.size() == 2);
  }
}
