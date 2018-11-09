// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StaticMemoryPool::size()") {
  SECTION("Capacity equals template parameter") {
    const size_t capacity = 64;
    StaticMemoryPool<capacity> memoryPool;
    REQUIRE(capacity == memoryPool.capacity());
  }

  SECTION("Initial size is 0") {
    StaticMemoryPool<32> memoryPool;
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("Increases after allocFrozenString()") {
    StaticMemoryPool<128> memoryPool;
    memoryPool.allocFrozenString(0);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(0));
    memoryPool.allocFrozenString(0);
    REQUIRE(memoryPool.size() == 2 * JSON_STRING_SIZE(0));
  }

  SECTION("Decreases after freeVariant()") {
    StaticMemoryPool<128> memoryPool;
    VariantSlot* a = memoryPool.allocVariant();
    VariantSlot* b = memoryPool.allocVariant();

    memoryPool.freeVariant(b);
    REQUIRE(memoryPool.size() == sizeof(VariantSlot));
    memoryPool.freeVariant(a);
    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Decreases after calling freeString() in order") {
    StaticMemoryPool<128> memoryPool;
    StringSlot* a = memoryPool.allocFrozenString(5);
    REQUIRE(a != 0);
    StringSlot* b = memoryPool.allocFrozenString(6);
    REQUIRE(b != 0);

    memoryPool.freeString(b);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(5));
    memoryPool.freeString(a);
    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Decreases after calling freeString() in reverse order") {
    StaticMemoryPool<128> memoryPool;
    StringSlot* a = memoryPool.allocFrozenString(5);
    REQUIRE(a != 0);
    StringSlot* b = memoryPool.allocFrozenString(6);
    REQUIRE(b != 0);

    memoryPool.freeString(a);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(6));
    memoryPool.freeString(b);
    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Doesn't grow when memory pool is full") {
    const size_t variantCount = 4;
    const size_t capacity = variantCount * sizeof(VariantSlot);
    StaticMemoryPool<capacity> memoryPool;

    for (size_t i = 0; i < variantCount; i++) memoryPool.allocVariant();
    REQUIRE(capacity == memoryPool.size());

    memoryPool.allocVariant();

    REQUIRE(capacity == memoryPool.size());
  }
}
