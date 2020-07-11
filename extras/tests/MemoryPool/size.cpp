// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemoryPool::capacity()") {
  char buffer[4096];
  const size_t capacity = 64;
  MemoryPool pool(buffer, capacity);
  REQUIRE(capacity == pool.capacity());
}

TEST_CASE("MemoryPool::size()") {
  char buffer[4096];
  MemoryPool pool(buffer, sizeof(buffer));

  SECTION("Initial size is 0") {
    REQUIRE(0 == pool.size());
  }

  SECTION("Decreases after freezeString()") {
    StringSlot a = pool.allocExpandableString();
    pool.freezeString(a, 1);
    REQUIRE(pool.size() == 1);

    StringSlot b = pool.allocExpandableString();
    pool.freezeString(b, 1);
    REQUIRE(pool.size() == 2);
  }

  SECTION("Increases after allocFrozenString()") {
    pool.allocFrozenString(1);
    REQUIRE(pool.size() == 1);

    pool.allocFrozenString(2);
    REQUIRE(pool.size() == 3);
  }

  SECTION("Doesn't grow when memory pool is full") {
    const size_t variantCount = sizeof(buffer) / sizeof(VariantSlot);

    for (size_t i = 0; i < variantCount; i++) pool.allocVariant();
    size_t size = pool.size();

    pool.allocVariant();

    REQUIRE(size == pool.size());
  }
}
