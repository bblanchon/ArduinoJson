// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(JsonArray &array, std::string expected) {
  std::string actual;
  size_t actualLen = array.printTo(actual);
  size_t measuredLen = array.measureLength();
  CHECK(actualLen == expected.size());
  CHECK(measuredLen == expected.size());
  REQUIRE(expected == actual);
}

TEST_CASE("JsonArray::printTo()") {
  StaticJsonBuffer<JSON_ARRAY_SIZE(2)> jb;
  JsonArray &array = jb.createArray();

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

  SECTION("OneDoubleDefaultDigits") {
    array.add(3.14159265358979323846);
    check(array, "[3.14]");
  }

  SECTION("OneDoubleFourDigits") {
    array.add(3.14159265358979323846, 4);
    check(array, "[3.1416]");
  }

  SECTION("OneDoubleFourDigits_AlternativeSyntax") {
    array.add(double_with_n_digits(3.14159265358979323846, 4));
    check(array, "[3.1416]");
  }

  SECTION("OneFloatDefaultDigits") {
    array.add(3.14159f);
    check(array, "[3.14]");
  }

  SECTION("OneFloatFourDigits") {
    array.add(3.14159f, 4);
    check(array, "[3.1416]");
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

  SECTION("RawJson") {
    array.add(RawJson("{\"key\":\"value\"}"));

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
