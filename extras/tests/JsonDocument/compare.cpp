// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonDocument::operator==(const DynamicJsonDocument&)") {
  DynamicJsonDocument doc1(4096);
  DynamicJsonDocument doc2(4096);

  SECTION("Empty") {
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }

  SECTION("With same object") {
    doc1["hello"] = "world";
    doc2["hello"] = "world";
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }
  SECTION("With different object") {
    doc1["hello"] = "world";
    doc2["world"] = "hello";
    REQUIRE_FALSE(doc1 == doc2);
    REQUIRE(doc1 != doc2);
  }
}

TEST_CASE("DynamicJsonDocument::operator==(const StaticJsonDocument&)") {
  DynamicJsonDocument doc1(4096);
  StaticJsonDocument<256> doc2;

  SECTION("Empty") {
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }

  SECTION("With same object") {
    doc1["hello"] = "world";
    doc2["hello"] = "world";
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }

  SECTION("With different object") {
    doc1["hello"] = "world";
    doc2["world"] = "hello";
    REQUIRE_FALSE(doc1 == doc2);
    REQUIRE(doc1 != doc2);
  }
}

TEST_CASE("StaticJsonDocument::operator==(const DynamicJsonDocument&)") {
  StaticJsonDocument<256> doc1;
  DynamicJsonDocument doc2(4096);

  SECTION("Empty") {
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }

  SECTION("With same object") {
    doc1["hello"] = "world";
    doc2["hello"] = "world";
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }

  SECTION("With different object") {
    doc1["hello"] = "world";
    doc2["world"] = "hello";
    REQUIRE_FALSE(doc1 == doc2);
    REQUIRE(doc1 != doc2);
  }
}
