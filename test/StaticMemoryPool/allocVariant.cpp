// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StaticMemoryPool::allocVariant()") {
  StaticMemoryPool<128> memoryPool;

  SECTION("Returns different pointer") {
    VariantSlot* s1 = memoryPool.allocVariant();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = memoryPool.allocVariant();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns same pointer after freeSlot()") {
    VariantSlot* s1 = memoryPool.allocVariant();
    memoryPool.freeVariant(s1);
    VariantSlot* s2 = memoryPool.allocVariant();

    REQUIRE(s1 == s2);
  }

  SECTION("Returns aligned pointers") {
    // make room for two
    // pass an uneven capacity
    StaticMemoryPool<2 * sizeof(VariantSlot) + 1> pool;

    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(isAligned(pool.allocVariant()));
  }
}
