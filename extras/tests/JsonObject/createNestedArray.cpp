// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::createNestedArray()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();

  SECTION("key is a const char*") {
    JsonArray arr = obj.createNestedArray("hello");
    REQUIRE(arr.isNull() == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    JsonArray arr = obj.createNestedArray(vla);
    REQUIRE(arr.isNull() == false);
  }
#endif
}
