// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

TEST_CASE("deserializeMsgPack(JsonDocument&)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc.add(std::string("hello"));
  spy.clearLog();

  auto err = deserializeMsgPack(doc, "\x91\x2A");

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc.as<std::string>() == "[42]");
  REQUIRE(spy.log() == AllocatorLog{
                           Deallocate(sizeofPool()),
                           Deallocate(sizeofString("hello")),
                           Allocate(sizeofPool()),
                           Reallocate(sizeofPool(), sizeofArray(1)),
                       });
}

TEST_CASE("deserializeMsgPack(JsonVariant)") {
  SECTION("variant is bound") {
    SpyingAllocator spy;
    JsonDocument doc(&spy);
    doc.add(std::string("hello"));
    spy.clearLog();

    JsonVariant variant = doc[0];

    auto err = deserializeMsgPack(variant, "\x91\x2A");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "[[42]]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("hello")),
                         });
  }

  SECTION("variant is unbound") {
    JsonVariant variant;

    auto err = deserializeMsgPack(variant, "\x91\x2A");

    REQUIRE(err == DeserializationError::NoMemory);
  }
}

TEST_CASE("deserializeMsgPack(ElementProxy)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc.add(std::string("hello"));
  spy.clearLog();

  SECTION("element already exists") {
    auto err = deserializeMsgPack(doc[0], "\x91\x2A");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "[[42]]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("hello")),
                         });
  }

  SECTION("element must be created exists") {
    auto err = deserializeMsgPack(doc[1], "\x91\x2A");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "[\"hello\",[42]]");
    REQUIRE(spy.log() == AllocatorLog{});
  }
}

TEST_CASE("deserializeMsgPack(MemberProxy)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc[std::string("hello")] = std::string("world");
  spy.clearLog();

  SECTION("member already exists") {
    auto err = deserializeMsgPack(doc["hello"], "\x91\x2A");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "{\"hello\":[42]}");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("member must be created") {
    auto err = deserializeMsgPack(doc["value"], "\x91\x2A");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\",\"value\":[42]}");
    REQUIRE(spy.log() == AllocatorLog{});
  }
}
