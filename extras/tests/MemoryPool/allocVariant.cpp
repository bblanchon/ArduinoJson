// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemoryPool::allocVariant()") {
  char buffer[4096];

  SECTION("Returns different pointer") {
    MemoryPool pool(buffer, sizeof(buffer));

    VariantSlot* s1 = pool.allocVariant();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = pool.allocVariant();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns aligned pointers") {
    MemoryPool pool(buffer, sizeof(buffer));

    REQUIRE(isAligned(pool.allocVariant()));
    REQUIRE(isAligned(pool.allocVariant()));
  }

  SECTION("Returns zero if capacity is 0") {
    MemoryPool pool(buffer, 0);

    REQUIRE(pool.allocVariant() == 0);
  }

  SECTION("Returns zero if buffer is null") {
    MemoryPool pool(0, sizeof(buffer));

    REQUIRE(pool.allocVariant() == 0);
  }

  SECTION("Returns zero if capacity is insufficient") {
    MemoryPool pool(buffer, sizeof(VariantSlot));

    pool.allocVariant();

    REQUIRE(pool.allocVariant() == 0);
  }
}
