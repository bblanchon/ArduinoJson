// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

static bool isAligned(void* ptr) {
  const size_t mask = sizeof(void*) - 1;
  size_t addr = reinterpret_cast<size_t>(ptr);
  return (addr & mask) == 0;
}

TEST_CASE("DynamicJsonBuffer::alloc()") {
  DynamicJsonBuffer buffer;

  SECTION("InitialSizeIsZero") {
    REQUIRE(0 == buffer.size());
  }

  SECTION("SizeIncreasesAfterAlloc") {
    buffer.alloc(1);
    REQUIRE(1U <= buffer.size());
    buffer.alloc(1);
    REQUIRE(2U <= buffer.size());
  }

  SECTION("ReturnDifferentPointer") {
    void* p1 = buffer.alloc(1);
    void* p2 = buffer.alloc(2);
    REQUIRE(p1 != p2);
  }

  SECTION("Alignment") {
    // make room for two but not three
    buffer = DynamicJsonBuffer(2 * sizeof(void*) + 1);

    REQUIRE(isAligned(buffer.alloc(1)));  // this on is aligned by design
    REQUIRE(isAligned(buffer.alloc(1)));  // this one fits in the first block
    REQUIRE(isAligned(buffer.alloc(1)));  // this one requires a new block
  }
}
