// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <sstream>

TEST_CASE("JsonString") {
  SECTION("Default constructor creates a null JsonString") {
    JsonString s;

    CHECK(s.isNull() == true);
    CHECK(s.c_str() == 0);
    CHECK(s.isStatic() == true);
  }

  SECTION("Compare null with boolean") {
    JsonString s;

    CHECK(bool(s) == false);
    CHECK(false == bool(s));
    CHECK(bool(s) != true);
    CHECK(true != bool(s));
  }

  SECTION("Compare non-null with boolean") {
    JsonString s("hello");
    CHECK(bool(s) == true);
    CHECK(true == bool(s));
    CHECK(bool(s) != false);
    CHECK(false != bool(s));
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

  SECTION("std::stream") {
    std::stringstream ss;
    ss << JsonString("hello world!");
    CHECK(ss.str() == "hello world!");
  }
}
