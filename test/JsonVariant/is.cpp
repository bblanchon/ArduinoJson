// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

void checkIsArray(JsonArray value) {
  DynamicJsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  REQUIRE(var.is<JsonArray>());
  REQUIRE(var.is<JsonArray>());
  REQUIRE(var.is<const JsonArray>());
  REQUIRE(var.is<const JsonArray>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<double>());
  REQUIRE_FALSE(var.is<float>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<const char *>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsBool(bool value) {
  DynamicJsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  REQUIRE(var.is<bool>());

  REQUIRE_FALSE(var.is<double>());
  REQUIRE_FALSE(var.is<float>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<const char *>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsFloat(double value) {
  DynamicJsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  REQUIRE(var.is<double>());
  REQUIRE(var.is<float>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<const char *>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

template <typename T>
void checkIsInteger(T value) {
  DynamicJsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  REQUIRE(var.is<long>());
  REQUIRE(var.is<int>());
  REQUIRE(var.is<float>());
  REQUIRE(var.is<double>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<const char *>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

void checkIsString(const char *value) {
  DynamicJsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  REQUIRE(var.is<const char *>());

  REQUIRE_FALSE(var.is<bool>());
  REQUIRE_FALSE(var.is<int>());
  REQUIRE_FALSE(var.is<double>());
  REQUIRE_FALSE(var.is<float>());
  REQUIRE_FALSE(var.is<long>());
  REQUIRE_FALSE(var.is<JsonArray>());
  REQUIRE_FALSE(var.is<JsonObject>());
}

TEST_CASE("JsonVariant::is()") {
  SECTION("JsonArray") {
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    checkIsArray(array);
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
}
