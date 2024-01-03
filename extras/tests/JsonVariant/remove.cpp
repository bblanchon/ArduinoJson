// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonVariant::remove(int)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("release top level strings") {
    doc.add(std::string("hello"));
    doc.add(std::string("hello"));
    doc.add(std::string("world"));

    JsonVariant var = doc.as<JsonVariant>();
    REQUIRE(var.as<std::string>() == "[\"hello\",\"hello\",\"world\"]");

    spy.clearLog();
    var.remove(1);
    REQUIRE(var.as<std::string>() == "[\"hello\",\"world\"]");
    REQUIRE(spy.log() == AllocatorLog{});

    spy.clearLog();
    var.remove(1);
    REQUIRE(var.as<std::string>() == "[\"hello\"]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("world")),
                         });

    spy.clearLog();
    var.remove(0);
    REQUIRE(var.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("hello")),
                         });
  }

  SECTION("release strings in nested array") {
    doc[0][0] = std::string("hello");

    JsonVariant var = doc.as<JsonVariant>();
    REQUIRE(var.as<std::string>() == "[[\"hello\"]]");

    spy.clearLog();
    var.remove(0);

    REQUIRE(var.as<std::string>() == "[]");
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofString("hello")),
                         });
  }
}

TEST_CASE("JsonVariant::remove(const char *)") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  var["a"] = 1;
  var["b"] = 2;

  var.remove("a");

  REQUIRE(var.as<std::string>() == "{\"b\":2}");
}

TEST_CASE("JsonVariant::remove(std::string)") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  var["a"] = 1;
  var["b"] = 2;

  var.remove(std::string("b"));

  REQUIRE(var.as<std::string>() == "{\"a\":1}");
}
