// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::containsKey()") {
  DynamicJsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj.set("hello", 42);

  SECTION("returns false if key not present") {
    REQUIRE(false == obj.containsKey("world"));
  }

  SECTION("returns true if key present") {
    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("returns false after remove()") {
    obj.remove("hello");

    REQUIRE(false == obj.containsKey("hello"));
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a VLA") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(true == obj.containsKey(vla));
  }
#endif
}
