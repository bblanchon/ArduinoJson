// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonVariant::size()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SUBCASE("unbound reference") {
    JsonVariant unbound;

    CHECK(unbound.size() == 0);
  }

  SUBCASE("int") {
    variant.set(42);

    CHECK(variant.size() == 0);
  }

  SUBCASE("string") {
    variant.set("hello");

    CHECK(variant.size() == 0);
  }

  SUBCASE("object") {
    variant["a"] = 1;
    variant["b"] = 2;

    CHECK(variant.size() == 2);
  }
}
