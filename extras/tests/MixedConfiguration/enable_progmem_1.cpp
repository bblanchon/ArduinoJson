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

TEST_CASE("strlen_P") {
  CHECK(strlen_P(FC("")) == 0);
  CHECK(strlen_P(FC("a")) == 1);
  CHECK(strlen_P(FC("ac")) == 2);
}

TEST_CASE("strncmp_P") {
  CHECK(strncmp_P("a", FC("b"), 0) == 0);
  CHECK(strncmp_P("a", FC("b"), 1) == -1);
  CHECK(strncmp_P("b", FC("a"), 1) == 1);
  CHECK(strncmp_P("a", FC("a"), 0) == 0);
  CHECK(strncmp_P("a", FC("b"), 2) == -1);
  CHECK(strncmp_P("b", FC("a"), 2) == 1);
  CHECK(strncmp_P("a", FC("a"), 2) == 0);
}

TEST_CASE("strcmp_P") {
  CHECK(strcmp_P("a", FC("b")) == -1);
  CHECK(strcmp_P("b", FC("a")) == 1);
  CHECK(strcmp_P("a", FC("a")) == 0);
  CHECK(strcmp_P("aa", FC("ab")) == -1);
  CHECK(strcmp_P("ab", FC("aa")) == 1);
  CHECK(strcmp_P("aa", FC("aa")) == 0);
}

TEST_CASE("memcpy_P") {
  char dst[4];
  CHECK(memcpy_P(dst, FC("ABC"), 4) == dst);
  CHECK(dst[0] == 'A');
  CHECK(dst[1] == 'B');
  CHECK(dst[2] == 'C');
  CHECK(dst[3] == 0);
}
