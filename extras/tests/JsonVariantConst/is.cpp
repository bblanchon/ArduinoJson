// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

enum MYENUM2 { ONE = 1, TWO = 2 };

TEST_CASE("JsonVariantConst::is<T>()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  SUBCASE("unbound") {
    var = JsonVariantConst();

    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonArrayConst>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonObjectConst>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonVariantConst>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("null") {
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("true") {
    doc.set(true);

    CHECK(var.is<bool>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("false") {
    doc.set(false);

    CHECK(var.is<bool>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("int") {
    doc.set(42);

    CHECK(var.is<int>() == true);
    CHECK(var.is<short>() == true);
    CHECK(var.is<long>() == true);
    CHECK(var.is<double>() == true);
    CHECK(var.is<float>() == true);
    CHECK(var.is<MYENUM2>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
  }

  SUBCASE("double") {
    doc.set(4.2);

    CHECK(var.is<double>() == true);
    CHECK(var.is<float>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("const char*") {
    doc.set("4.2");

    CHECK(var.is<const char*>() == true);
    CHECK(var.is<const char*>() == true);
    CHECK(var.is<std::string>() == true);
    CHECK(var.is<JsonString>() == true);
    CHECK(var.is<double>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("JsonArray") {
    doc.to<JsonArray>();

    CHECK(var.is<JsonArrayConst>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonObjectConst>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SUBCASE("JsonObject") {
    doc.to<JsonObject>();

    CHECK(var.is<JsonObjectConst>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonArrayConst>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }
}
