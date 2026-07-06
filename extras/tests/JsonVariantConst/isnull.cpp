// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonVariantConst::isNull()") {
  JsonDocument doc;
  JsonVariantConst variant = doc.to<JsonVariant>();

  SUBCASE("returns true when undefined") {
    REQUIRE(variant.isNull() == true);
  }

  SUBCASE("returns false if value is integer") {
    doc.set(42);

    REQUIRE(variant.isNull() == false);
  }
}
