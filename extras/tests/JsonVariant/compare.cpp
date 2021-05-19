// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

// Most code is already covered by arithmeticCompare.cpp.
// Here, we're just filling the holes

TEST_CASE("Compare JsonVariant with value") {
  StaticJsonDocument<256> doc;
  JsonVariant a = doc.addElement();

  SECTION("null vs (char*)0") {
    char* b = 0;

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("42 vs 42") {
    a.set(42);
    int b = 42;

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }
}

TEST_CASE("Compare JsonVariant with JsonVariant") {
  StaticJsonDocument<256> doc;
  JsonVariant a = doc.addElement();
  JsonVariant b = doc.addElement();

  SECTION("'abc' vs 'abc'") {
    a.set("abc");
    b.set("abc");

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("'abc' vs 'bcd'") {
    a.set("abc");
    b.set("bcd");

    CHECK(a != b);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK_FALSE(a == b);
    CHECK_FALSE(a > b);
    CHECK_FALSE(a >= b);
  }

  SECTION("'bcd' vs 'abc'") {
    a.set("bcd");
    b.set("abc");

    CHECK(a != b);
    CHECK(a > b);
    CHECK(a >= b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
  }

  SECTION("serialized('abc') vs serialized('abc')") {
    a.set(serialized("abc"));
    b.set(serialized("abc"));

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("serialized('abc') vs serialized('bcd')") {
    a.set(serialized("abc"));
    b.set(serialized("bcd"));

    CHECK(a != b);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK_FALSE(a == b);
    CHECK_FALSE(a > b);
    CHECK_FALSE(a >= b);
  }

  SECTION("serialized('bcd') vs serialized('abc')") {
    a.set(serialized("bcd"));
    b.set(serialized("abc"));

    CHECK(a != b);
    CHECK(a > b);
    CHECK(a >= b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
  }

  SECTION("false vs true") {
    a.set(false);
    b.set(true);

    CHECK(a != b);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK_FALSE(a == b);
    CHECK_FALSE(a > b);
    CHECK_FALSE(a >= b);
  }

  SECTION("false vs -1") {
    a.set(false);
    b.set(-1);

    CHECK(a != b);
    CHECK(a > b);
    CHECK(a >= b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
  }

  SECTION("null vs null") {
    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("42 vs 42") {
    a.set(42);
    b.set(42);

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("42 vs 42.0") {
    a.set(42);
    b.set(42.0);

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("42.0 vs 42") {
    a.set(42.0);
    b.set(42);

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("-42 vs -42") {
    a.set(-42);
    b.set(-42);

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("-42 vs 42") {
    a.set(-42);
    b.set(42);

    CHECK(a != b);
    CHECK(a < b);
    CHECK(a <= b);
    CHECK_FALSE(a == b);
    CHECK_FALSE(a > b);
    CHECK_FALSE(a >= b);
  }

  SECTION("42 vs -42") {
    a.set(42);
    b.set(-42);

    CHECK(a != b);
    CHECK(a > b);
    CHECK(a >= b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
  }

  SECTION("42.0 vs -42") {
    a.set(42.0);
    b.set(-42);

    CHECK(a != b);
    CHECK(a > b);
    CHECK(a >= b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
  }

  SECTION("[1] vs [1]") {
    a.add(1);
    b.add(1);

    CHECK(a <= b);
    CHECK(a == b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("[1] vs [2]") {
    a.add(1);
    b.add(2);

    CHECK(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
    CHECK_FALSE(a > b);
    CHECK_FALSE(a >= b);
  }

  SECTION("{x:1} vs {x:1}") {
    a["x"] = 1;
    b["x"] = 1;

    CHECK(a <= b);
    CHECK(a == b);
    CHECK(a >= b);
    CHECK_FALSE(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a > b);
  }

  SECTION("{x:1} vs {x:2}") {
    a["x"] = 1;
    b["x"] = 2;

    CHECK(a != b);
    CHECK_FALSE(a < b);
    CHECK_FALSE(a <= b);
    CHECK_FALSE(a == b);
    CHECK_FALSE(a > b);
    CHECK_FALSE(a >= b);
  }
}
