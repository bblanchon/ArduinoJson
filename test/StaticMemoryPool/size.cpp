// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("StaticMemoryPool::size()") {
  StaticMemoryPool<64> memoryPool;

  SECTION("Capacity equals template parameter") {
    REQUIRE(64 == memoryPool.capacity());
  }

  SECTION("Initial size is 0") {
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("Increases after alloc()") {
    memoryPool.alloc(1);
    REQUIRE(1U <= memoryPool.size());
    memoryPool.alloc(1);
    REQUIRE(2U <= memoryPool.size());
  }

  SECTION("Doesn't grow when memoryPool is full") {
    memoryPool.alloc(64);
    memoryPool.alloc(1);
    REQUIRE(64 == memoryPool.size());
  }

  SECTION("Does't grow when memoryPool is too small for alloc") {
    memoryPool.alloc(65);
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("Goes back to zero after clear()") {
    memoryPool.alloc(1);
    memoryPool.clear();
    REQUIRE(0 == memoryPool.size());
  }
}
