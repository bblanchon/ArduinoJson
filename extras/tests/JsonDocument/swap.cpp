#include <ArduinoJson.h>

#include <doctest.h>
#include <string>
#include <utility>

using namespace std;

TEST_CASE("std::swap") {
  SUBCASE("JsonDocument*") {
    JsonDocument *p1, *p2;
    swap(p1, p2);  // issue #1678
  }

  SUBCASE("JsonDocument") {
    JsonDocument doc1, doc2;
    doc1.set("hello");
    doc2.set("world");

    swap(doc1, doc2);

    CHECK(doc1.as<string>() == "world");
    CHECK(doc2.as<string>() == "hello");
  }
}
