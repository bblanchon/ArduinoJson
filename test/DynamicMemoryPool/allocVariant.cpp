// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("DynamicMemoryPool::allocVariant()") {
  DynamicMemoryPool memoryPool;

  SECTION("Returns different pointer") {
    VariantSlot* s1 = memoryPool.allocVariant();
    VariantSlot* s2 = memoryPool.allocVariant();

    REQUIRE(s1 != s2);
  }

  SECTION("Returns same pointer after freeSlot()") {
    VariantSlot* s1 = memoryPool.allocVariant();
    memoryPool.freeVariant(s1);
    VariantSlot* s2 = memoryPool.allocVariant();

    REQUIRE(s1 == s2);
  }

  SECTION("Returns aligned pointers") {
    // make room for two but not three
    // pass an uneven capacity
    DynamicMemoryPool pool(2 * sizeof(VariantSlot) + 1);

    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(pool.blockCount() == 1);

    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(pool.blockCount() == 2);
  }
}
