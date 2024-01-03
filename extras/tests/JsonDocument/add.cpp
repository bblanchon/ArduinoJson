// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonDocument::add(T)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("integer") {
    doc.add(42);

    REQUIRE(doc.as<std::string>() == "[42]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }

  SECTION("const char*") {
    doc.add("hello");

    REQUIRE(doc.as<std::string>() == "[\"hello\"]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }

  SECTION("std::string") {
    doc.add(std::string("example"));
    doc.add(std::string("example"));

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("char*") {
    char value[] = "example";
    doc.add(value);
    doc.add(value);

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("Arduino String") {
    doc.add(String("example"));
    doc.add(String("example"));

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SECTION("Flash string") {
    doc.add(F("example"));
    doc.add(F("example"));

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }
}

TEST_CASE("JsonDocument::add<T>()") {
  JsonDocument doc;

  SECTION("JsonArray") {
    JsonArray array = doc.add<JsonArray>();
    array.add(1);
    array.add(2);
    REQUIRE(doc.as<std::string>() == "[[1,2]]");
  }

  SECTION("JsonObject") {
    JsonObject object = doc.add<JsonObject>();
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("JsonVariant") {
    JsonVariant variant = doc.add<JsonVariant>();
    variant.set(42);
    REQUIRE(doc.as<std::string>() == "[42]");
  }
}
