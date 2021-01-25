// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::createNestedObject()") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();

  SECTION("key is a const char*") {
    obj.createNestedObject("hello");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a VLA") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    obj.createNestedObject(vla);
  }
#endif
}
