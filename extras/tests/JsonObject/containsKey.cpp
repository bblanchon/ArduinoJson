// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::containsKey()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();
  obj["hello"] = 42;

  SECTION("returns true only if key is present") {
    REQUIRE(false == obj.containsKey("world"));
    REQUIRE(true == obj.containsKey("hello"));
  }

  SECTION("works with JsonObjectConst") {
    JsonObjectConst cobj = obj;
    REQUIRE(false == cobj.containsKey("world"));
    REQUIRE(true == cobj.containsKey("hello"));
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
