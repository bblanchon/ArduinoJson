// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

enum MYENUM2 { ONE = 1, TWO = 2 };

TEST_CASE("JsonVariant::is<T>()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("unbound") {
    variant = JsonVariant();

    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<JsonVariant>() == false);
    CHECK(variant.is<JsonVariantConst>() == false);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<std::string>() == false);
    CHECK(variant.is<JsonString>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<MYENUM2>() == false);
    CHECK(variant.is<JsonString>() == false);
  }

  SECTION("null") {
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<std::string>() == false);
    CHECK(variant.is<JsonString>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<MYENUM2>() == false);
  }

  SECTION("true") {
    variant.set(true);

    CHECK(variant.is<bool>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<std::string>() == false);
    CHECK(variant.is<JsonString>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<MYENUM2>() == false);
  }

  SECTION("false") {
    variant.set(false);

    CHECK(variant.is<bool>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<std::string>() == false);
    CHECK(variant.is<JsonString>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<MYENUM2>() == false);
  }

  SECTION("int") {
    variant.set(42);

    CHECK(variant.is<int>() == true);
    CHECK(variant.is<short>() == true);
    CHECK(variant.is<long>() == true);
    CHECK(variant.is<double>() == true);
    CHECK(variant.is<float>() == true);
    CHECK(variant.is<MYENUM2>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<std::string>() == false);
    CHECK(variant.is<JsonString>() == false);
  }

  SECTION("double") {
    variant.set(4.2);

    CHECK(variant.is<double>() == true);
    CHECK(variant.is<float>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<std::string>() == false);
    CHECK(variant.is<JsonString>() == false);
    CHECK(variant.is<MYENUM2>() == false);
  }

  SECTION("const char*") {
    variant.set("4.2");

    CHECK(variant.is<const char*>() == true);
    CHECK(variant.is<const char*>() == true);
    CHECK(variant.is<std::string>() == true);
    CHECK(variant.is<JsonString>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<double>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<MYENUM2>() == false);
  }

  SECTION("JsonArray") {
    variant.to<JsonArray>();

    CHECK(variant.is<JsonArray>() == true);
    CHECK(variant.is<JsonArrayConst>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<JsonObject>() == false);
    CHECK(variant.is<JsonObjectConst>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<MYENUM2>() == false);
  }

  SECTION("JsonObject") {
    variant.to<JsonObject>();

    CHECK(variant.is<JsonObject>() == true);
    CHECK(variant.is<JsonObjectConst>() == true);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
    CHECK(variant.is<JsonArray>() == false);
    CHECK(variant.is<JsonArrayConst>() == false);
    CHECK(variant.is<int>() == false);
    CHECK(variant.is<float>() == false);
    CHECK(variant.is<bool>() == false);
    CHECK(variant.is<const char*>() == false);
    CHECK(variant.is<MYENUM2>() == false);
    CHECK(variant.is<JsonVariant>() == true);
    CHECK(variant.is<JsonVariantConst>() == true);
  }
}

TEST_CASE("JsonVariantConst::is<T>()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  JsonVariantConst cvariant = variant;

  SECTION("unbound") {
    cvariant = JsonVariantConst();

    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonArrayConst>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonObjectConst>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<JsonVariantConst>() == false);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<std::string>() == false);
    CHECK(cvariant.is<JsonString>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("null") {
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<std::string>() == false);
    CHECK(cvariant.is<JsonString>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("true") {
    variant.set(true);

    CHECK(cvariant.is<bool>() == true);
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<std::string>() == false);
    CHECK(cvariant.is<JsonString>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("false") {
    variant.set(false);

    CHECK(cvariant.is<bool>() == true);
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<std::string>() == false);
    CHECK(cvariant.is<JsonString>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("int") {
    variant.set(42);

    CHECK(cvariant.is<int>() == true);
    CHECK(cvariant.is<short>() == true);
    CHECK(cvariant.is<long>() == true);
    CHECK(cvariant.is<double>() == true);
    CHECK(cvariant.is<float>() == true);
    CHECK(cvariant.is<MYENUM2>() == true);
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<std::string>() == false);
    CHECK(cvariant.is<JsonString>() == false);
  }

  SECTION("double") {
    variant.set(4.2);

    CHECK(cvariant.is<double>() == true);
    CHECK(cvariant.is<float>() == true);
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<std::string>() == false);
    CHECK(cvariant.is<JsonString>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("const char*") {
    variant.set("4.2");

    CHECK(cvariant.is<const char*>() == true);
    CHECK(cvariant.is<const char*>() == true);
    CHECK(cvariant.is<std::string>() == true);
    CHECK(cvariant.is<JsonString>() == true);
    CHECK(cvariant.is<double>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("JsonArray") {
    variant.to<JsonArray>();

    CHECK(cvariant.is<JsonArrayConst>() == true);
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonObjectConst>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }

  SECTION("JsonObject") {
    variant.to<JsonObject>();

    CHECK(cvariant.is<JsonObjectConst>() == true);
    CHECK(cvariant.is<JsonVariantConst>() == true);
    CHECK(cvariant.is<JsonObject>() == false);
    CHECK(cvariant.is<JsonVariant>() == false);
    CHECK(cvariant.is<JsonArray>() == false);
    CHECK(cvariant.is<JsonArrayConst>() == false);
    CHECK(cvariant.is<int>() == false);
    CHECK(cvariant.is<float>() == false);
    CHECK(cvariant.is<bool>() == false);
    CHECK(cvariant.is<const char*>() == false);
    CHECK(cvariant.is<MYENUM2>() == false);
  }
}
