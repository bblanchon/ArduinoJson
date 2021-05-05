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

  SECTION("JsonVariant::is<char*>()") {
    JsonVariant v = doc["s"];
    REQUIRE(v.is<char*>() == true);
  }

  SECTION("JsonVariant::is<char>()") {
    JsonVariant v = doc["c"];
    REQUIRE(v.is<char>() == true);
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

  SECTION("JsonVariantConst::is<char*>()") {
    JsonVariantConst v = doc["s"];
    REQUIRE(v.is<char*>() == true);
  }

  SECTION("JsonVariantConst::is<char>()") {
    JsonVariantConst v = doc["c"];
    REQUIRE(v.is<char>() == true);
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

  SECTION("MemberProxy::is<char*>()") {
    REQUIRE(doc["s"].is<char*>() == true);
    REQUIRE(doc["c"].is<char*>() == false);
  }

  SECTION("MemberProxy::is<char>()") {
    REQUIRE(doc["c"].is<char>() == true);
    REQUIRE(doc["s"].is<char>() == false);
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

  SECTION("ElementProxy::is<char*>()") {
    REQUIRE(doc["a"][0].is<char*>() == true);
    REQUIRE(doc["a"][1].is<char*>() == false);
  }

  SECTION("ElementProxy::is<char>()") {
    REQUIRE(doc["a"][1].is<char>() == true);
    REQUIRE(doc["a"][0].is<char>() == false);
  }
}
