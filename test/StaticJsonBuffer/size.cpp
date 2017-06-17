// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("StaticJsonBuffer::size()") {
  StaticJsonBuffer<64> buffer;

  SECTION("Capacity equals template parameter") {
    REQUIRE(64 == buffer.capacity());
  }

  SECTION("Initial size is 0") {
    REQUIRE(0 == buffer.size());
  }

  SECTION("Increases after alloc()") {
    buffer.alloc(1);
    REQUIRE(1U <= buffer.size());
    buffer.alloc(1);
    REQUIRE(2U <= buffer.size());
  }

  SECTION("Doesn't grow when buffer is full") {
    buffer.alloc(64);
    buffer.alloc(1);
    REQUIRE(64 == buffer.size());
  }

  SECTION("Does't grow when buffer is too small for alloc") {
    buffer.alloc(65);
    REQUIRE(0 == buffer.size());
  }

  SECTION("Goes back to zero after clear()") {
    buffer.alloc(1);
    buffer.clear();
    REQUIRE(0 == buffer.size());
  }
}
