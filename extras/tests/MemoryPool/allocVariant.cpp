// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("MemoryPool::allocVariant()") {
  SECTION("Returns different pointer") {
    MemoryPool pool(4096);

    VariantSlot* s1 = pool.allocVariant();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = pool.allocVariant();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns aligned pointers") {
    MemoryPool pool(4096);

    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(isAligned(pool.allocVariant()));
  }

  SECTION("Returns zero if capacity is 0") {
    MemoryPool pool(0);

    REQUIRE(pool.allocVariant() == 0);
  }

  SECTION("Returns zero if buffer is null") {
    MemoryPool pool(4096, FailingAllocator::instance());

    REQUIRE(pool.allocVariant() == 0);
  }

  SECTION("Returns zero if capacity is insufficient") {
    MemoryPool pool(sizeof(VariantSlot));

    pool.allocVariant();

    REQUIRE(pool.allocVariant() == 0);
  }
}
