// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("DynamicMemoryPool::size()") {
  DynamicMemoryPool memoryPool;

  SECTION("Initial size is 0") {
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("Increases after alloc()") {
    memoryPool.alloc(1);
    REQUIRE(1U <= memoryPool.size());
    memoryPool.alloc(1);
    REQUIRE(2U <= memoryPool.size());
  }

  SECTION("Goes back to 0 after clear()") {
    memoryPool.alloc(1);
    memoryPool.clear();
    REQUIRE(0 == memoryPool.size());
  }
}
