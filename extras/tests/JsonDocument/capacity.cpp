// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument") {
  JsonDocument doc(4096);

  SECTION("capacity()") {
    SECTION("matches constructor argument") {
      JsonDocument doc2(256);
      REQUIRE(doc2.capacity() == 256);
    }

    SECTION("rounds up constructor argument") {
      JsonDocument doc2(253);
      REQUIRE(doc2.capacity() == 256);
    }
  }
}
