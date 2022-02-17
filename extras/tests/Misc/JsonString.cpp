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
    CHECK(s.isLinked() == true);
    CHECK(s == JsonString());
    CHECK(s != "");
  }

  SECTION("Null converts to false") {
    JsonString s;

    CHECK(bool(s) == false);
  }

  SECTION("Empty string converts to true") {
    JsonString s("");

    CHECK(bool(s) == true);
  }

  SECTION("Non-empty string converts to true") {
    JsonString s("");

    CHECK(bool(s) == true);
  }

  SECTION("Null strings equals each others") {
    JsonString a, b;

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SECTION("Null and empty strings differ") {
    JsonString a, b("");

    CHECK_FALSE(a == b);
    CHECK(a != b);

    CHECK_FALSE(b == a);
    CHECK(b != a);
  }

  SECTION("Null and non-empty strings differ") {
    JsonString a, b("hello");

    CHECK_FALSE(a == b);
    CHECK(a != b);

    CHECK_FALSE(b == a);
    CHECK(b != a);
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

  SECTION("Construct with a size") {
    JsonString s("hello world", 5);

    CHECK(s.size() == 5);
    CHECK(s.isLinked() == true);
    CHECK(s == "hello");
    CHECK(s != "hello world");
  }
}
