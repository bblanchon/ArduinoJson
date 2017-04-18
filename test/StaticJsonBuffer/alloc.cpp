// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

static bool isAligned(void *ptr) {
  const size_t mask = sizeof(void *) - 1;
  size_t addr = reinterpret_cast<size_t>(ptr);
  return (addr & mask) == 0;
}

TEST_CASE("StaticJsonBuffer::alloc()") {
  StaticJsonBuffer<64> buffer;

  SECTION("CapacityMatchTemplateParameter") {
    REQUIRE(64 == buffer.capacity());
  }

  SECTION("InitialSizeIsZero") {
    REQUIRE(0 == buffer.size());
  }

  SECTION("GrowsAfterAlloc") {
    buffer.alloc(1);
    REQUIRE(1U <= buffer.size());
    buffer.alloc(1);
    REQUIRE(2U <= buffer.size());
  }

  SECTION("DoesntGrowWhenFull") {
    buffer.alloc(64);
    buffer.alloc(1);
    REQUIRE(64 == buffer.size());
  }

  SECTION("DoesntGrowWhenTooSmall") {
    buffer.alloc(65);
    REQUIRE(0 == buffer.size());
  }

  SECTION("ReturnsNonNull") {
    void *p = buffer.alloc(64);
    REQUIRE(0 != p);
  }

  SECTION("ReturnsNullWhenFull") {
    buffer.alloc(64);
    void *p = buffer.alloc(1);
    REQUIRE(0 == p);
  }

  SECTION("ReturnsNullWhenTooSmall") {
    void *p = buffer.alloc(65);
    REQUIRE(0 == p);
  }

  SECTION("Alignment") {
    for (size_t size = 1; size <= sizeof(void *); size++) {
      void *p = buffer.alloc(1);
      REQUIRE(isAligned(p));
    }
  }
}
