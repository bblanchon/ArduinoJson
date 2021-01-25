// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonString") {
  SECTION("Default constructor creates a null JsonString") {
    JsonString s;

    CHECK(s.isNull() == true);
    CHECK(s.c_str() == 0);
    CHECK(s.isStatic() == true);
  }

  SECTION("Compare null with null") {
    JsonString a, b;

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SECTION("Compare null with non-null") {
    JsonString a(0), b("hello");

    CHECK_FALSE(a == b);
    CHECK(a != b);
  }

  SECTION("Compare non-null with null") {
    JsonString a("hello"), b(0);

    CHECK_FALSE(a == b);
    CHECK(a != b);
  }

  SECTION("Compare different strings") {
    JsonString a("hello"), b("world");

    CHECK_FALSE(a == b);
    CHECK(a != b);
  }

  SECTION("Compare identical by pointer") {
    JsonString a("hello"), b("hello");

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SECTION("Compare identical by value") {
    char s1[] = "hello";
    char s2[] = "hello";
    JsonString a(s1), b(s2);

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }
}
