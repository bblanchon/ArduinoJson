// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("JsonObjectConst::operator[]") {
  JsonDocument doc;
  doc["hello"] = "world";
  JsonObjectConst obj = doc.as<JsonObjectConst>();

  SECTION("supports const char*") {
    REQUIRE(obj["hello"] == "world");  // issue #2019
  }

  SECTION("supports std::string") {
    REQUIRE(obj[std::string("hello")] == "world");  // issue #2019
  }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(std::string("world") == obj[vla]);
  }
#endif
}
