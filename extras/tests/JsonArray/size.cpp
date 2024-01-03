// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::size()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();

  SECTION("returns 0 is empty") {
    REQUIRE(0U == array.size());
  }

  SECTION("increases after add()") {
    array.add("hello");
    REQUIRE(1U == array.size());

    array.add("world");
    REQUIRE(2U == array.size());
  }

  SECTION("remains the same after replacing an element") {
    array.add("hello");
    REQUIRE(1U == array.size());

    array[0] = "hello";
    REQUIRE(1U == array.size());
  }
}
