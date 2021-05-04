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
  doc["a"].add(s);

  SECTION("JsonVariant::as<char*>()") {
    JsonVariant v = doc["s"];
    REQUIRE(v.as<char*>() == s);
  }

  SECTION("JsonVariantConst::as<char*>()") {
    JsonVariantConst v = doc["s"];
    REQUIRE(v.as<char*>() == s);
  }

  SECTION("MemberProxy::as<char*>()") {
    REQUIRE(doc["s"].as<char*>() == s);
  }

  SECTION("ElementProxy::as<char*>()") {
    REQUIRE(doc["a"][0].as<char*>() == s);
  }
}
