// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
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

TEST_CASE("BoundedReader<const __FlashStringHelper*>") {
  using namespace ARDUINOJSON_NAMESPACE;

  SECTION("read") {
    BoundedReader<const __FlashStringHelper*> reader(F("\x01\xFF"), 2);
    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == -1);
    REQUIRE(reader.read() == -1);
  }

  SECTION("readBytes() all at once") {
    BoundedReader<const __FlashStringHelper*> reader(F("ABCD"), 3);

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 4) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    BoundedReader<const __FlashStringHelper*> reader(F("ABCDEF"), 6);

    char buffer[8] = "abcdefg";
    REQUIRE(reader.readBytes(buffer, 4) == 4);
    REQUIRE(reader.readBytes(buffer + 4, 4) == 2);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'D');
    REQUIRE(buffer[4] == 'E');
    REQUIRE(buffer[5] == 'F');
    REQUIRE(buffer[6] == 'g');
  }
}

TEST_CASE("Reader<const __FlashStringHelper*>") {
  using namespace ARDUINOJSON_NAMESPACE;

  SECTION("read()") {
    Reader<const __FlashStringHelper*> reader(F("\x01\xFF\x00\x12"));
    REQUIRE(reader.read() == 0x01);
    REQUIRE(reader.read() == 0xFF);
    REQUIRE(reader.read() == 0);
    REQUIRE(reader.read() == 0x12);
  }

  SECTION("readBytes() all at once") {
    Reader<const __FlashStringHelper*> reader(F("ABCD"));

    char buffer[8] = "abcd";
    REQUIRE(reader.readBytes(buffer, 3) == 3);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'd');
  }

  SECTION("readBytes() in two parts") {
    Reader<const __FlashStringHelper*> reader(F("ABCDEF"));

    char buffer[8] = "abcdefg";
    REQUIRE(reader.readBytes(buffer, 4) == 4);
    REQUIRE(reader.readBytes(buffer + 4, 2) == 2);

    REQUIRE(buffer[0] == 'A');
    REQUIRE(buffer[1] == 'B');
    REQUIRE(buffer[2] == 'C');
    REQUIRE(buffer[3] == 'D');
    REQUIRE(buffer[4] == 'E');
    REQUIRE(buffer[5] == 'F');
    REQUIRE(buffer[6] == 'g');
  }
}
