// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonDocument::add()") {
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
