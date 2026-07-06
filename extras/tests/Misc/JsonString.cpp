// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include <sstream>

TEST_CASE("JsonString") {
  SUBCASE("Default constructor creates a null JsonString") {
    JsonString s;

    CHECK(s.isNull() == true);
    CHECK(s.c_str() == nullptr);
    CHECK(s == JsonString());
    CHECK(s != "");
  }

  SUBCASE("Null converts to false") {
    JsonString s;

    CHECK(bool(s) == false);
  }

  SUBCASE("Empty string converts to true") {
    JsonString s("");

    CHECK(bool(s) == true);
  }

  SUBCASE("Non-empty string converts to true") {
    JsonString s("");

    CHECK(bool(s) == true);
  }

  SUBCASE("Null strings equals each others") {
    JsonString a, b;

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SUBCASE("Null and empty strings differ") {
    JsonString a, b("");

    CHECK_FALSE(a == b);
    CHECK(a != b);

    CHECK_FALSE(b == a);
    CHECK(b != a);
  }

  SUBCASE("Null and non-empty strings differ") {
    JsonString a, b("hello");

    CHECK_FALSE(a == b);
    CHECK(a != b);

    CHECK_FALSE(b == a);
    CHECK(b != a);
  }

  SUBCASE("Compare different strings") {
    JsonString a("hello"), b("world");

    CHECK_FALSE(a == b);
    CHECK(a != b);
  }

  SUBCASE("Compare identical by pointer") {
    JsonString a("hello"), b("hello");

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SUBCASE("Compare identical by value") {
    char s1[] = "hello";
    char s2[] = "hello";
    JsonString a(s1), b(s2);

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SUBCASE("std::stream") {
    std::stringstream ss;
    ss << JsonString("hello world!");
    CHECK(ss.str() == "hello world!");
  }

  SUBCASE("Construct with a size") {
    JsonString s("hello world", 5);

    CHECK(s.size() == 5);
    CHECK(s == "hello");
    CHECK(s != "hello world");
  }
}
