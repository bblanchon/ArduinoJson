// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::createNestedArray()") {
  DynamicJsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("key is a const char*") {
    obj.createNestedArray("hello");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a VLA") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    obj.createNestedArray(vla);
  }
#endif
}
