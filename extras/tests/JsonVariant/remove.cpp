// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("JsonVariant::remove(int)") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("release top level strings") {
    doc.add("hello"_s);
    doc.add("hello"_s);
    doc.add("world"_s);

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
    doc[0][0] = "hello"_s;

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

  var.remove("b"_s);

  REQUIRE(var.as<std::string>() == "{\"a\":1}");
}

TEST_CASE("JsonVariant::remove(JsonVariant) from object") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  var["a"] = "a";
  var["b"] = 2;
  var["c"] = "b";

  var.remove(var["c"]);

  REQUIRE(var.as<std::string>() == "{\"a\":\"a\",\"c\":\"b\"}");
}

TEST_CASE("JsonVariant::remove(JsonVariant) from array") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  var[0] = 3;
  var[1] = 2;
  var[2] = 1;

  var.remove(var[2]);
  var.remove(var[3]);  // noop

  REQUIRE(var.as<std::string>() == "[3,1]");
}
