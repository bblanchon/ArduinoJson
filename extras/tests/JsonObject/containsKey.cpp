// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::containsKey()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["hello"] = 42;

  SECTION("returns true only if key is present") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("returns false after remove()") {
    obj.remove("hello");

    REQUIRE(false == obj.containsKey("hello"));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif

  SECTION("key is a JsonVariant") {
    doc["key"] = "hello";
    REQUIRE(true == obj.containsKey(obj["key"]));
    REQUIRE(false == obj.containsKey(obj["hello"]));
  }
}
