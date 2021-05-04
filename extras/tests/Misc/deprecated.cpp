// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_DEPRECATED(msg)  // nothing

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Deprecated features") {
  StaticJsonDocument<256> doc;
  const char* s = "hello";
  doc["s"] = s;
  doc["c"] = 42;
  doc["a"].add(s);
  doc["a"].add(42);

  SECTION("JsonVariant::add(char)") {
    JsonVariant v = doc.to<JsonVariant>();
    v.add('*');
    REQUIRE(v[0] == 42);
  }

  SECTION("JsonVariant::as<char*>()") {
    JsonVariant v = doc["s"];
    REQUIRE(v.as<char*>() == s);
  }

  SECTION("JsonVariant::as<char>()") {
    JsonVariant v = doc["c"];
    REQUIRE(v.as<char>() == '*');
  }

  SECTION("JsonVariant::set(char)") {
    JsonVariant v = doc.to<JsonVariant>();
    v.set('*');
    REQUIRE(v.as<unsigned char>() == 42);
  }

  SECTION("JsonVariantConst::as<char*>()") {
    JsonVariantConst v = doc["s"];
    REQUIRE(v.as<char*>() == s);
  }

  SECTION("JsonVariantConst::as<char>()") {
    JsonVariantConst v = doc["c"];
    REQUIRE(v.as<char>() == '*');
  }

  SECTION("MemberProxy::as<char*>()") {
    REQUIRE(doc["s"].as<char*>() == s);
  }

  SECTION("MemberProxy::as<char>()") {
    REQUIRE(doc["c"].as<char>() == '*');
  }

  SECTION("MemberProxy::as<char>()") {
    doc["x"].set('*');
    REQUIRE(doc["x"] == 42);
  }

  SECTION("ElementProxy::as<char*>()") {
    REQUIRE(doc["a"][0].as<char*>() == s);
  }

  SECTION("ElementProxy::as<char>()") {
    REQUIRE(doc["a"][1].as<char>() == '*');
  }

  SECTION("ElementProxy::as<char>()") {
    doc["a"][0].set('*');
    REQUIRE(doc["a"][0] == 42);
  }
}
