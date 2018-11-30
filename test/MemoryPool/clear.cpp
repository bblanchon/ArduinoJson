// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

static const size_t poolCapacity = 512;

TEST_CASE("MemoryPool::clear()") {
  char buffer[poolCapacity];
  MemoryPool memoryPool(buffer, sizeof(buffer));

  SECTION("Discards allocated variants") {
    memoryPool.allocVariant();

    memoryPool.clear();
    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Discards allocated strings") {
    memoryPool.allocFrozenString(10);
    REQUIRE(memoryPool.size() > 0);

    memoryPool.clear();

    REQUIRE(memoryPool.size() == 0);
  }
}
