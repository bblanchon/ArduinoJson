// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename TVariant>
void checkIsArray(TVariant var) {
  REQUIRE(var.template is<JsonArray>());

  REQUIRE_FALSE(var.template is<bool>());
  REQUIRE_FALSE(var.template is<double>());
  REQUIRE_FALSE(var.template is<float>());
  REQUIRE_FALSE(var.template is<int>());
  REQUIRE_FALSE(var.template is<long>());
  REQUIRE_FALSE(var.template is<const char *>());
  REQUIRE_FALSE(var.template is<JsonObject>());
}

void testArray(JsonArray value) {
  DynamicJsonDocument doc(4096);

  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  checkIsArray(var);

  JsonVariantConst cvar = var;
  checkIsArray(cvar);
}

template <typename TVariant>
void checkIsBool(TVariant var) {
  REQUIRE(var.template is<bool>());

  REQUIRE_FALSE(var.template is<double>());
  REQUIRE_FALSE(var.template is<float>());
  REQUIRE_FALSE(var.template is<int>());
  REQUIRE_FALSE(var.template is<long>());
  REQUIRE_FALSE(var.template is<const char *>());
  REQUIRE_FALSE(var.template is<JsonArray>());
  REQUIRE_FALSE(var.template is<JsonObject>());
}

void testBool(bool value) {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  checkIsBool(var);
  checkIsBool(JsonVariantConst(var));
}

template <typename TVariant>
void checkIsFloat(TVariant var) {
  REQUIRE(var.template is<double>());
  REQUIRE(var.template is<float>());

  REQUIRE_FALSE(var.template is<bool>());
  REQUIRE_FALSE(var.template is<int>());
  REQUIRE_FALSE(var.template is<long>());
  REQUIRE_FALSE(var.template is<const char *>());
  REQUIRE_FALSE(var.template is<JsonArray>());
  REQUIRE_FALSE(var.template is<JsonObject>());
}

void testFloat(double value) {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  checkIsFloat(var);
  checkIsFloat(JsonVariantConst(var));
}

template <typename TVariant>
void checkIsInteger(TVariant var) {
  REQUIRE(var.template is<long>());
  REQUIRE(var.template is<int>());
  REQUIRE(var.template is<float>());
  REQUIRE(var.template is<double>());

  REQUIRE_FALSE(var.template is<bool>());
  REQUIRE_FALSE(var.template is<const char *>());
  REQUIRE_FALSE(var.template is<JsonArray>());
  REQUIRE_FALSE(var.template is<JsonObject>());
}

template <typename T>
void testInteger(T value) {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  checkIsInteger(var);
  checkIsInteger(JsonVariantConst(var));
}

template <typename TVariant>
void checkIsString(TVariant var) {
  REQUIRE(var.template is<const char *>());
  REQUIRE(var.template is<std::string>());

  REQUIRE_FALSE(var.template is<bool>());
  REQUIRE_FALSE(var.template is<int>());
  REQUIRE_FALSE(var.template is<double>());
  REQUIRE_FALSE(var.template is<float>());
  REQUIRE_FALSE(var.template is<long>());
  REQUIRE_FALSE(var.template is<JsonArray>());
  REQUIRE_FALSE(var.template is<JsonObject>());
}

void testString(const char *value) {
  DynamicJsonDocument doc(4096);
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);

  checkIsString(var);
  checkIsString(JsonVariantConst(var));
}

TEST_CASE("JsonVariant::is()") {
  SECTION("JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    testArray(array);
  }

  SECTION("bool") {
    testBool(true);
    testBool(false);
  }

  SECTION("double") {
    testFloat(4.2);
  }

  SECTION("int") {
    testInteger(42);
  }

  SECTION("long") {
    testInteger(42L);
  }

  SECTION("string") {
    testString("42");
  }

  SECTION("null") {
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, "[null]");
    JsonVariant v = doc[0];

    REQUIRE(v.is<bool>() == false);
    REQUIRE(v.is<char *>() == false);
    REQUIRE(v.is<int>() == false);
    REQUIRE(v.is<std::string>() == false);
    REQUIRE(v.is<float>() == false);
  }
}
