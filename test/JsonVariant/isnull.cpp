// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::isNull()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("return true when Undefined") {
    REQUIRE(variant.isNull() == true);
  }

  SECTION("return false when Integer") {
    variant.set(42);

    REQUIRE(variant.isNull() == false);
  }

  SECTION("return false when EmptyArray") {
    DynamicJsonDocument doc2(4096);
    JsonArray array = doc2.to<JsonArray>();

    variant.set(array);
    REQUIRE(variant.isNull() == false);
  }

  SECTION("return false when EmptyObject") {
    DynamicJsonDocument doc2(4096);
    JsonObject obj = doc2.to<JsonObject>();

    variant.set(obj);
    REQUIRE(variant.isNull() == false);
  }

  /*  SECTION("return true when InvalidArray") {
      variant.set(JsonArray());
      REQUIRE(variant.isNull() == true);
    }
  */
  /*  SECTION("return true when InvalidObject") {
      variant.set(JsonObject());
      REQUIRE(variant.isNull() == true);
    }*/

  SECTION("works with JsonVariantConst") {
    variant.set(42);

    JsonVariantConst cvar = variant;

    REQUIRE(cvar.isNull() == false);
  }
}
