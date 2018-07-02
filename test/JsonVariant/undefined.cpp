// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant undefined") {
  JsonVariant variant;

  SECTION("AsLongReturns0") {
    REQUIRE(0 == variant.as<long>());
  }

  SECTION("AsUnsignedReturns0") {
    REQUIRE(0 == variant.as<unsigned>());
  }

  SECTION("AsStringReturnsNull") {
    REQUIRE(0 == variant.as<char*>());
  }

  SECTION("AsDoubleReturns0") {
    REQUIRE(0 == variant.as<double>());
  }

  SECTION("AsBoolReturnsFalse") {
    REQUIRE(false == variant.as<bool>());
  }

  SECTION("AsArrayReturnInvalid") {
    REQUIRE(JsonArray() == variant.as<JsonArray>());
  }

  SECTION("AsConstArrayReturnInvalid") {
    REQUIRE(JsonArray() == variant.as<const JsonArray>());
  }

  SECTION("AsObjectReturnInvalid") {
    REQUIRE(JsonObject() == variant.as<JsonObject>());
  }

  SECTION("AsConstObjectReturnInvalid") {
    REQUIRE(JsonObject() == variant.as<const JsonObject>());
  }

  SECTION("AsArrayWrapperReturnInvalid") {
    REQUIRE(JsonArray() == variant.as<JsonArray>());
  }

  SECTION("AsObjectWrapperReturnInvalid") {
    REQUIRE(JsonObject() == variant.as<JsonObject>());
  }
}
