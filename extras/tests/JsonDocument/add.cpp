// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

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
    doc.add("example"_s);
    doc.add("example"_s);

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

  SECTION("JsonVariant") {
    JsonVariant variant = doc.add<JsonVariant>();
    variant.set(42);
    REQUIRE(doc.as<std::string>() == "[42]");
  }
}

TEST_CASE("JsonObject::add(JsonObject) ") {
  JsonDocument doc1;
  doc1["hello"_s] = "world"_s;

  TimebombAllocator allocator(10);
  SpyingAllocator spy(&allocator);
  JsonDocument doc2(&spy);

  SECTION("success") {
    bool result = doc2.add(doc1.as<JsonObject>());

    REQUIRE(result == true);
    REQUIRE(doc2.as<std::string>() == "[{\"hello\":\"world\"}]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("partial failure") {  // issue #2081
    allocator.setCountdown(2);

    bool result = doc2.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             AllocateFail(sizeofString("world")),
                             Deallocate(sizeofString("hello")),
                         });
  }

  SECTION("complete failure") {
    allocator.setCountdown(0);

    bool result = doc2.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofPool()),
                         });
  }
}
