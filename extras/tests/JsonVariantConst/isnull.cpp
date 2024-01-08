// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariantConst::isNull()") {
  JsonDocument doc;
  JsonVariantConst variant = doc.to<JsonVariant>();

  SECTION("returns true when undefined") {
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns false if value is integer") {
    doc.set(42);

    REQUIRE(variant.isNull() == false);
  }
}
