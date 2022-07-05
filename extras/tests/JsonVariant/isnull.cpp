// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::isNull()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("returns true when Undefined") {
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns false when Integer") {
    variant.set(42);

    REQUIRE(variant.isNull() == false);
  }

  SECTION("returns false when EmptyArray") {
    DynamicJsonDocument doc2(4096);
    JsonArray array = doc2.to<JsonArray>();

    variant.set(array);
    REQUIRE(variant.isNull() == false);
  }

  SECTION("returns false when EmptyObject") {
    DynamicJsonDocument doc2(4096);
    JsonObject obj = doc2.to<JsonObject>();

    variant.set(obj);
    REQUIRE(variant.isNull() == false);
  }

  SECTION("returns true after set(JsonArray())") {
    variant.set(JsonArray());
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns true after set(JsonObject())") {
    variant.set(JsonObject());
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns false after set('hello')") {
    variant.set("hello");
    REQUIRE(variant.isNull() == false);
  }

  SECTION("returns true after set((char*)0)") {
    variant.set(static_cast<char*>(0));
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns true after set((const char*)0)") {
    variant.set(static_cast<const char*>(0));
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns true after set(serialized((char*)0))") {
    variant.set(serialized(static_cast<char*>(0)));
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns true after set(serialized((const char*)0))") {
    variant.set(serialized(static_cast<const char*>(0)));
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns true for a shallow null copy") {
    StaticJsonDocument<128> doc2;
    variant.shallowCopy(doc2);
    CHECK(variant.isNull() == true);
  }

  SECTION("returns false for a shallow array copy") {
    StaticJsonDocument<128> doc2;
    doc2[0] = 42;
    variant.shallowCopy(doc2);
    CHECK(variant.isNull() == false);
  }

  SECTION("works with JsonVariantConst") {
    variant.set(42);

    JsonVariantConst cvar = variant;

    REQUIRE(cvar.isNull() == false);
  }
}
