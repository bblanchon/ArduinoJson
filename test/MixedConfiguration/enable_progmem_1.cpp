// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include "progmem_emulation.hpp"

#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("Flash strings") {
  DynamicJsonDocument doc(2048);

  SECTION("deserializeJson()") {
    DeserializationError err = deserializeJson(doc, F("{'hello':'world'}"));

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc["hello"] == "world");
  }

  SECTION("JsonDocument::operator[]") {
    doc[F("hello")] = F("world");

    REQUIRE(doc["hello"] == "world");
  }

  SECTION("JsonDocument::add()") {
    doc.add(F("world"));

    REQUIRE(doc[0] == "world");
  }

  SECTION("JsonVariant::set()") {
    JsonVariant var = doc.to<JsonVariant>();

    var.set(F("world"));

    REQUIRE(var == "world");
  }

  SECTION("MemberProxy::operator==") {
    doc["hello"] = "world";

    REQUIRE(doc["hello"] == F("world"));
  }

  SECTION("ElementProxy::operator==") {
    doc.add("world");

    REQUIRE(doc[0] == F("world"));
  }
}
