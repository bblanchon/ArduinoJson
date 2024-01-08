// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObjectConst::containsKey()") {
  JsonDocument doc;
  doc["hello"] = 42;
  auto obj = doc.as<JsonObjectConst>();

  SECTION("supports const char*") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("supports std::string") {
    REQUIRE(false == obj.containsKey(std::string("world")));
    REQUIRE(true == obj.containsKey(std::string("hello")));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif
}
