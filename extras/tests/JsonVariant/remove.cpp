// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonVariant::remove(int)") {
  JsonDocument doc;

  SECTION("release top level strings") {
    doc.add(std::string("hello"));
    doc.add(std::string("hello"));
    doc.add(std::string("world"));

    JsonVariant var = doc.as<JsonVariant>();
    REQUIRE(var.as<std::string>() == "[\"hello\",\"hello\",\"world\"]");
    REQUIRE(doc.memoryUsage() == sizeofArray(3) + 2 * sizeofString(5));

    var.remove(1);
    REQUIRE(var.as<std::string>() == "[\"hello\",\"world\"]");
    REQUIRE(doc.memoryUsage() == sizeofArray(3) + 2 * sizeofString(5));

    var.remove(1);
    REQUIRE(var.as<std::string>() == "[\"hello\"]");
    REQUIRE(doc.memoryUsage() == sizeofArray(3) + 1 * sizeofString(5));

    var.remove(0);
    REQUIRE(var.as<std::string>() == "[]");
    REQUIRE(doc.memoryUsage() == sizeofArray(3));
  }

  SECTION("release strings in nested array") {
    doc[0][0] = std::string("hello");

    JsonVariant var = doc.as<JsonVariant>();
    REQUIRE(var.as<std::string>() == "[[\"hello\"]]");
    REQUIRE(doc.memoryUsage() == 2 * sizeofArray(1) + sizeofString(5));

    var.remove(0);
    REQUIRE(var.as<std::string>() == "[]");
    REQUIRE(doc.memoryUsage() == 2 * sizeofArray(1));
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
