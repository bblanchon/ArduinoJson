#include <ArduinoJson.h>

#include <catch.hpp>
#include <utility>

using namespace std;

TEST_CASE("std::swap") {
  SECTION("DynamicJsonDocument*") {
    DynamicJsonDocument *p1, *p2;
    swap(p1, p2);  // issue #1678
  }
}
