#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>
#include <utility>

using namespace std;

TEST_CASE("std::swap") {
  SECTION("DynamicJsonDocument*") {
    DynamicJsonDocument *p1, *p2;
    swap(p1, p2);  // issue #1678
  }

  SECTION("DynamicJsonDocument") {
    DynamicJsonDocument doc1(0x10), doc2(0x20);
    doc1.set("hello");
    doc2.set("world");

    swap(doc1, doc2);

    CHECK(doc1.capacity() == 0x20);
    CHECK(doc1.as<string>() == "world");
    CHECK(doc2.capacity() == 0x10);
    CHECK(doc2.as<string>() == "hello");
  }
}
