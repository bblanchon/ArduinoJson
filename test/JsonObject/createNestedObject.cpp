// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::createNestedObject()") {
  DynamicJsonDocument doc;
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

  SECTION("releases memory from nested object") {
    obj.createNestedObject(std::string("a"))
        .createNestedObject(std::string("b"))
        .set(std::string("c"))
        .set(1);
    // {"a":{"b":{"c":1}}}
    REQUIRE(doc.memoryUsage() ==
            3 * JSON_OBJECT_SIZE(1) + 3 * JSON_STRING_SIZE(2));

    obj.createNestedObject(std::string("a"));
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(1) + JSON_STRING_SIZE(2));
  }
}
