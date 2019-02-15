// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(JsonArray& array, std::string expected) {
  std::string actual;
  size_t actualLen = array.prettyPrintTo(actual);
  size_t measuredLen = array.measurePrettyLength();
  CHECK(actualLen == expected.size());
  CHECK(measuredLen == expected.size());
  REQUIRE(expected == actual);
}

TEST_CASE("JsonArray::prettyPrintTo()") {
  DynamicJsonBuffer jb;
  JsonArray& array = jb.createArray();

  SECTION("Empty") {
    check(array, "[]");
  }

  SECTION("OneElement") {
    array.add(1);

    check(array,
          "[\r\n"
          "  1\r\n"
          "]");
  }

  SECTION("TwoElements") {
    array.add(1);
    array.add(2);

    check(array,
          "[\r\n"
          "  1,\r\n"
          "  2\r\n"
          "]");
  }

  SECTION("EmptyNestedArrays") {
    array.createNestedArray();
    array.createNestedArray();

    check(array,
          "[\r\n"
          "  [],\r\n"
          "  []\r\n"
          "]");
  }

  SECTION("NestedArrays") {
    JsonArray& nested1 = array.createNestedArray();
    nested1.add(1);
    nested1.add(2);

    JsonObject& nested2 = array.createNestedObject();
    nested2["key"] = 3;

    check(array,
          "[\r\n"
          "  [\r\n"
          "    1,\r\n"
          "    2\r\n"
          "  ],\r\n"
          "  {\r\n"
          "    \"key\": 3\r\n"
          "  }\r\n"
          "]");
  }
}
