// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonObjectConst::operator[]") {
  JsonDocument doc;
  doc["hello"] = "world";
  doc["a\0b"_s] = "ABC";
  JsonObjectConst obj = doc.as<JsonObjectConst>();

  SECTION("supports const char*") {
    REQUIRE(obj["hello"] == "world");  // issue #2019
  }

  SECTION("supports std::string") {
    REQUIRE(obj["hello"_s] == "world");  // issue #2019
    REQUIRE(obj["a\0b"_s] == "ABC");
  }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(obj[vla] == "world"_s);
  }
#endif

  SECTION("supports JsonVariant") {
    doc["key1"] = "hello";
    doc["key2"] = "a\0b"_s;
    doc["key3"] = "foo";
    REQUIRE(obj[obj["key1"]] == "world");
    REQUIRE(obj[obj["key2"]] == "ABC");
    REQUIRE(obj[obj["key3"]] == nullptr);
    REQUIRE(obj[obj["key4"]] == nullptr);
  }
}
