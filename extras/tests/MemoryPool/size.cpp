// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("MemoryPool::capacity()") {
  const size_t capacity = 64;
  MemoryPool pool(capacity);
  REQUIRE(capacity == pool.capacity());
}

TEST_CASE("MemoryPool::size()") {
  MemoryPool pool(4096);

  SECTION("Initial size is 0") {
    REQUIRE(0 == pool.size());
  }

  SECTION("Doesn't grow when memory pool is full") {
    const size_t variantCount = pool.capacity() / sizeof(VariantSlot);

    for (size_t i = 0; i < variantCount; i++)
      pool.allocVariant();
    size_t size = pool.size();

    pool.allocVariant();

    REQUIRE(size == pool.size());
  }
}
