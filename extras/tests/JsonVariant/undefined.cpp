// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant undefined") {
  JsonVariant variant;

  SECTION("as<T>()") {
    SECTION("long") {
      REQUIRE(variant.as<long>() == 0);
    }

    SECTION("unsigned") {
      REQUIRE(variant.as<unsigned>() == 0);
    }

    SECTION("const char*") {
      REQUIRE(variant.as<const char*>() == 0);
    }

    SECTION("double") {
      REQUIRE(variant.as<double>() == 0);
    }

    SECTION("bool") {
      REQUIRE(variant.as<bool>() == false);
    }

    SECTION("JsonArray") {
      REQUIRE(variant.as<JsonArray>().isNull());
    }

    SECTION("JsonObject") {
      REQUIRE(variant.as<JsonObject>().isNull());
    }
  }

  SECTION("is<T>()") {
    SECTION("long") {
      REQUIRE(variant.is<long>() == false);
    }

    SECTION("unsigned") {
      REQUIRE(variant.is<unsigned>() == false);
    }

    SECTION("const char*") {
      REQUIRE(variant.is<const char*>() == false);
    }

    SECTION("double") {
      REQUIRE(variant.is<double>() == false);
    }

    SECTION("bool") {
      REQUIRE(variant.is<bool>() == false);
    }

    SECTION("JsonArray") {
      REQUIRE(variant.is<JsonArray>() == false);
    }

    SECTION("JsonObject") {
      REQUIRE(variant.is<JsonObject>() == false);
    }
  }

  SECTION("set<T>()") {
    SECTION("long") {
      REQUIRE(variant.set(42L) == false);
    }

    SECTION("unsigned") {
      REQUIRE(variant.set(42U) == false);
    }

    SECTION("const char*") {
      REQUIRE(variant.set("42") == false);
    }

    SECTION("Serialized<const char*>") {
      REQUIRE(variant.set(serialized("42")) == false);
    }

    SECTION("double") {
      REQUIRE(variant.set(42.0) == false);
    }

    SECTION("bool") {
      REQUIRE(variant.set(true) == false);
    }
  }
}
