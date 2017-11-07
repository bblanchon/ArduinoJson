// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonBuffer::size()") {
  DynamicJsonBuffer buffer;

  SECTION("Initial size is 0") {
    REQUIRE(0 == buffer.size());
  }

  SECTION("Increases after alloc()") {
    buffer.alloc(1);
    REQUIRE(1U <= buffer.size());
    buffer.alloc(1);
    REQUIRE(2U <= buffer.size());
  }

  SECTION("Goes back to 0 after clear()") {
    buffer.alloc(1);
    buffer.clear();
    REQUIRE(0 == buffer.size());
  }
}
