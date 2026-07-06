// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

TEST_CASE("JsonDocument::operator==(const JsonDocument&)") {
  JsonDocument doc1;
  JsonDocument doc2;

  SUBCASE("Empty") {
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }

  SUBCASE("With same object") {
    doc1["hello"] = "world";
    doc2["hello"] = "world";
    REQUIRE(doc1 == doc2);
    REQUIRE_FALSE(doc1 != doc2);
  }
  SUBCASE("With different object") {
    doc1["hello"] = "world";
    doc2["world"] = "hello";
    REQUIRE_FALSE(doc1 == doc2);
    REQUIRE(doc1 != doc2);
  }
}
