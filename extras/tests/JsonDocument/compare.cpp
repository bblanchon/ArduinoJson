// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
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

TEST_CASE("JsonDocument::operator==(const JsonDocument&)") {
  DynamicJsonDocument doc1(256);
  DynamicJsonDocument doc2(256);
  const JsonDocument& ref1 = doc1;
  const JsonDocument& ref2 = doc2;

  SECTION("Empty") {
    REQUIRE(ref1 == ref2);
    REQUIRE_FALSE(ref1 != ref2);
  }

  SECTION("With same object") {
    doc1["hello"] = "world";
    doc2["hello"] = "world";
    REQUIRE(ref1 == ref2);
    REQUIRE_FALSE(ref1 != ref2);
  }

  SECTION("With different object") {
    doc1["hello"] = "world";
    doc2["world"] = "hello";
    REQUIRE_FALSE(ref1 == ref2);
    REQUIRE(ref1 != ref2);
  }
}
