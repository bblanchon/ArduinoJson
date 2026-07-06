// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_ENABLE_PROGMEM 1
#include <ArduinoJson.h>

#include <doctest.h>

#include "Allocators.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonDocument::add(T)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SUBCASE("integer") {
    doc.add(42);

    REQUIRE(doc.as<std::string>() == "[42]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }

  SUBCASE("string literal") {
    doc.add("hello");

    REQUIRE(doc.as<std::string>() == "[\"hello\"]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SUBCASE("const char*") {
    const char* value = "hello";
    doc.add(value);

    REQUIRE(doc.as<std::string>() == "[\"hello\"]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                         });
  }

  SUBCASE("std::string") {
    doc.add("example"_s);
    doc.add("example"_s);

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("char*") {
    char value[] = "example";
    doc.add(value);
    doc.add(value);

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("Arduino String") {
    doc.add(String("example"));
    doc.add(String("example"));

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

  SUBCASE("Flash string") {
    doc.add(F("example"));
    doc.add(F("example"));

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "example");

    doc.add(vla);
    doc.add(vla);

    CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
    REQUIRE("example"_s == doc[0]);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("example")),
                         });
  }
#endif
}

TEST_CASE("JsonDocument::add<T>()") {
  JsonDocument doc;

  SUBCASE("JsonArray") {
    JsonArray array = doc.add<JsonArray>();
    array.add(1);
    array.add(2);
    REQUIRE(doc.as<std::string>() == "[[1,2]]");
  }

  SUBCASE("JsonVariant") {
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

  SUBCASE("success") {
    bool result = doc2.add(doc1.as<JsonObject>());

    REQUIRE(result == true);
    REQUIRE(doc2.as<std::string>() == "[{\"hello\":\"world\"}]");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofString("world")),
                         });
  }

  SUBCASE("partial failure") {  // issue #2081
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

  SUBCASE("complete failure") {
    allocator.setCountdown(0);

    bool result = doc2.add(doc1.as<JsonObject>());

    REQUIRE(result == false);
    REQUIRE(doc2.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofPool()),
                         });
  }
}
