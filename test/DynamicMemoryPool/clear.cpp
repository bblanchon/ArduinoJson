// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StaticMemoryPool::clear()") {
  DynamicMemoryPool memoryPool;

  SECTION("Discards allocated variants") {
    memoryPool.allocVariant();
    REQUIRE(memoryPool.size() > 0);

    memoryPool.clear();
    CHECK(memoryPool.size() == 0);
  }

  SECTION("Discards allocated strings") {
    memoryPool.allocFrozenString(10);
    REQUIRE(memoryPool.size() > 0);

    memoryPool.clear();

    CHECK(memoryPool.size() == 0);
  }

  SECTION("Purges variant cache") {
    memoryPool.freeVariant(memoryPool.allocVariant());
    REQUIRE(memoryPool.size() == 0);

    memoryPool.clear();

    CHECK(memoryPool.size() == 0);
  }

  SECTION("Purges string cache") {
    memoryPool.freeString(memoryPool.allocFrozenString(10));
    // REQUIRE(memoryPool.size() == 0);

    memoryPool.clear();

    CHECK(memoryPool.size() == 0);
  }
}
