// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

char buffer[4096];

TEST_CASE("MemoryPool::capacity()") {
  const size_t capacity = 64;
  MemoryPool memoryPool(buffer, capacity);
  REQUIRE(capacity == memoryPool.capacity());
}

TEST_CASE("MemoryPool::size()") {
  MemoryPool memoryPool(buffer, sizeof(buffer));

  SECTION("Initial size is 0") {
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("size() == capacity() after allocExpandableString()") {
    memoryPool.allocExpandableString();
    REQUIRE(memoryPool.size() == memoryPool.capacity());
  }

  SECTION("Decreases after freezeString()") {
    StringSlot* a = memoryPool.allocExpandableString();
    memoryPool.freezeString(a, 1);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(1));

    StringSlot* b = memoryPool.allocExpandableString();
    memoryPool.freezeString(b, 1);
    REQUIRE(memoryPool.size() == 2 * JSON_STRING_SIZE(1));
  }

  SECTION("Increases after allocFrozenString()") {
    memoryPool.allocFrozenString(0);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(0));

    memoryPool.allocFrozenString(0);
    REQUIRE(memoryPool.size() == 2 * JSON_STRING_SIZE(0));
  }

  SECTION("Doesn't grow when memory pool is full") {
    const size_t variantCount = sizeof(buffer) / sizeof(VariantSlot);

    for (size_t i = 0; i < variantCount; i++) memoryPool.allocVariant();
    size_t size = memoryPool.size();

    memoryPool.allocVariant();

    REQUIRE(size == memoryPool.size());
  }
}
