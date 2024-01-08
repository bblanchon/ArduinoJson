// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::isNull()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("returns true when Undefined") {
    REQUIRE(variant.isNull() == true);
  }

  SECTION("returns false when Integer") {
    variant.set(42);

    REQUIRE(variant.isNull() == false);
  }

  SECTION("returns false when EmptyArray") {
    JsonDocument doc2;
    JsonArray array = doc2.to<JsonArray>();

    variant.set(array);
    REQUIRE(variant.isNull() == false);
  }

  SECTION("returns false when EmptyObject") {
    JsonDocument doc2;
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
}
