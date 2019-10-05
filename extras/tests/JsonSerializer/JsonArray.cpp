// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(JsonArray array, std::string expected) {
  std::string actual;
  size_t actualLen = serializeJson(array, actual);
  REQUIRE(expected == actual);
  REQUIRE(actualLen == expected.size());
  size_t measuredLen = measureJson(array);
  REQUIRE(measuredLen == expected.size());
}

TEST_CASE("serializeJson(JsonArray)") {
  StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;
  JsonArray array = doc.to<JsonArray>();

  SECTION("Empty") {
    check(array, "[]");
  }

  SECTION("Null") {
    array.add(static_cast<char *>(0));

    check(array, "[null]");
  }

  SECTION("OneString") {
    array.add("hello");

    check(array, "[\"hello\"]");
  }

  SECTION("TwoStrings") {
    array.add("hello");
    array.add("world");

    check(array, "[\"hello\",\"world\"]");
  }

  SECTION("OneStringOverCapacity") {
    array.add("hello");
    array.add("world");
    array.add("lost");

    check(array, "[\"hello\",\"world\"]");
  }

  SECTION("One double") {
    array.add(3.1415927);
    check(array, "[3.1415927]");
  }

  SECTION("OneInteger") {
    array.add(1);

    check(array, "[1]");
  }

  SECTION("TwoIntegers") {
    array.add(1);
    array.add(2);

    check(array, "[1,2]");
  }

  SECTION("serialized(const char*)") {
    array.add(serialized("{\"key\":\"value\"}"));

    check(array, "[{\"key\":\"value\"}]");
  }

  SECTION("serialized(char*)") {
    char tmp[] = "{\"key\":\"value\"}";
    array.add(serialized(tmp));

    check(array, "[{\"key\":\"value\"}]");
  }

  SECTION("OneIntegerOverCapacity") {
    array.add(1);
    array.add(2);
    array.add(3);

    check(array, "[1,2]");
  }

  SECTION("OneTrue") {
    array.add(true);

    check(array, "[true]");
  }

  SECTION("OneFalse") {
    array.add(false);

    check(array, "[false]");
  }

  SECTION("TwoBooleans") {
    array.add(false);
    array.add(true);

    check(array, "[false,true]");
  }

  SECTION("OneBooleanOverCapacity") {
    array.add(false);
    array.add(true);
    array.add(false);

    check(array, "[false,true]");
  }

  SECTION("OneEmptyNestedArray") {
    array.createNestedArray();

    check(array, "[[]]");
  }

  SECTION("OneEmptyNestedHash") {
    array.createNestedObject();

    check(array, "[{}]");
  }
}
