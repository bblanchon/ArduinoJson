// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

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
