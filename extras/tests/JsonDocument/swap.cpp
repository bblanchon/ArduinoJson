#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>
#include <utility>

using namespace std;

TEST_CASE("std::swap") {
  SECTION("JsonDocument*") {
    JsonDocument *p1, *p2;
    swap(p1, p2);  // issue #1678
  }

  SECTION("JsonDocument") {
    JsonDocument doc1, doc2;
    doc1.set("hello");
    doc2.set("world");

    swap(doc1, doc2);

    CHECK(doc1.as<string>() == "world");
    CHECK(doc2.as<string>() == "hello");
  }
}
