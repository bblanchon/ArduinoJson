// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("new (pool) VariantSlot()") {
  SECTION("Returns different pointer") {
    MemoryPool pool(4096);

    VariantSlot* s1 = new (&pool) VariantSlot();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = new (&pool) VariantSlot();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns aligned pointers") {
    MemoryPool pool(4096);

    REQUIRE(isAligned(new (&pool) VariantSlot()));
    REQUIRE(isAligned(new (&pool) VariantSlot()));
  }

  SECTION("Returns zero if capacity is 0") {
    MemoryPool pool(0);

    REQUIRE(new (&pool) VariantSlot() == 0);
  }

  SECTION("Returns zero if buffer is null") {
    MemoryPool pool(4096, FailingAllocator::instance());

    REQUIRE(new (&pool) VariantSlot() == 0);
  }

  SECTION("Returns zero if capacity is insufficient") {
    MemoryPool pool(sizeof(VariantSlot));

    new (&pool) VariantSlot();

    REQUIRE(new (&pool) VariantSlot() == 0);
  }
}
