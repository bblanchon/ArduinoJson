// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>

#include <doctest.h>
#include <string>

#include "Allocators.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

TEST_CASE("deserializeJson(JsonDocument&)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc.add("hello"_s);
  spy.clearLog();

  auto err = deserializeJson(doc, "[42]");

  REQUIRE(err == DeserializationError::Ok);
  REQUIRE(doc.as<std::string>() == "[42]");
  REQUIRE(spy.log() == AllocatorLog{
                           Deallocate(sizeofPool()),
                           Deallocate(sizeofString("hello")),
                           Allocate(sizeofPool()),
                           Reallocate(sizeofPool(), sizeofArray(1)),
                       });
}

TEST_CASE("deserializeJson(JsonVariant)") {
  SUBCASE("variant is bound") {
    SpyingAllocator spy;
    JsonDocument doc(&spy);
    doc.add("hello"_s);
    spy.clearLog();

    JsonVariant variant = doc[0];

    auto err = deserializeJson(variant, "[42]");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "[[42]]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("hello")),
                         });
  }

  SUBCASE("variant is unbound") {
    JsonVariant variant;

    auto err = deserializeJson(variant, "[42]");

    REQUIRE(err == DeserializationError::NoMemory);
  }
}

TEST_CASE("deserializeJson(ElementProxy)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc.add("hello"_s);
  spy.clearLog();

  SUBCASE("element already exists") {
    auto err = deserializeJson(doc[0], "[42]");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "[[42]]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("hello")),
                         });
  }

  SUBCASE("element must be created") {
    auto err = deserializeJson(doc[1], "[42]");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "[\"hello\",[42]]");
    REQUIRE(spy.log() == AllocatorLog{});
  }
}

TEST_CASE("deserializeJson(MemberProxy)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  doc["hello"_s] = "world"_s;
  spy.clearLog();

  SUBCASE("member already exists") {
    auto err = deserializeJson(doc["hello"], "[42]");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "{\"hello\":[42]}");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });
  }

  SUBCASE("member must be created exists") {
    auto err = deserializeJson(doc["value"], "[42]");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\",\"value\":[42]}");
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("value")),
                         });
  }
}
