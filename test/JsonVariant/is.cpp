// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

void checkIsArray(JsonVariant var) {
  REQUIRE(var.is<JsonArray>());
  REQUIRE(var.is<JsonArray&>());
  REQUIRE(var.is<const JsonArray>());
  REQUIRE(var.is<const JsonArray&>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<double>());
  REQUIRE_FALSE(var.is<float>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<const char*>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsBool(JsonVariant var) {
  REQUIRE(var.is<bool>());

  REQUIRE_FALSE(var.is<double>());
  REQUIRE_FALSE(var.is<float>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<const char*>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsFloat(JsonVariant var) {
  REQUIRE(var.is<double>());
  REQUIRE(var.is<float>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<const char*>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsInteger(JsonVariant var) {
  REQUIRE(var.is<long>());
  REQUIRE(var.is<int>());
  REQUIRE(var.is<float>());
  REQUIRE(var.is<double>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<const char*>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsString(JsonVariant var) {
  REQUIRE(var.is<const char*>());
  REQUIRE(var.is<std::string>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<double>());
  REQUIRE_FALSE(var.is<float>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

TEST_CASE("JsonVariant::is()") {
  DynamicJsonBuffer jsonBuffer;

  SECTION("JsonArray") {
    checkIsArray(jsonBuffer.createArray());
  }

  SECTION("bool") {
    checkIsBool(true);
    checkIsBool(false);
  }

  SECTION("double") {
    checkIsFloat(4.2);
  }

  SECTION("int") {
    checkIsInteger(42);
  }

  SECTION("long") {
    checkIsInteger(42L);
  }

  SECTION("string") {
    checkIsString("42");
  }

  SECTION("unparsed bool") {
    checkIsBool(RawJson("true"));
    checkIsBool(RawJson("false"));
  }

  SECTION("unparsed int") {
    checkIsInteger(RawJson("42"));
  }

  SECTION("unparsed float") {
    checkIsFloat(RawJson("4.2e-10"));
  }

  SECTION("unparsed null") {
    checkIsString(RawJson("null"));
  }
}
