// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemoryPool::allocFrozenString()") {
  const size_t poolCapacity = 64;
  const size_t longestString = poolCapacity;
  char buffer[poolCapacity];
  MemoryPool pool(buffer, poolCapacity);

  SECTION("Returns different addresses") {
    char *a = pool.allocFrozenString(1);
    char *b = pool.allocFrozenString(1);
    REQUIRE(a != b);
  }

  SECTION("Returns NULL when full") {
    void *p1 = pool.allocFrozenString(longestString);
    REQUIRE(p1 != 0);

    void *p2 = pool.allocFrozenString(1);
    REQUIRE(p2 == 0);
  }

  SECTION("Returns NULL when pool is too small") {
    void *p = pool.allocFrozenString(longestString + 1);
    REQUIRE(0 == p);
  }

  SECTION("Returns NULL when buffer is NULL") {
    MemoryPool pool2(0, poolCapacity);
    REQUIRE(0 == pool2.allocFrozenString(2));
  }

  SECTION("Returns NULL when capacity is 0") {
    MemoryPool pool2(buffer, 0);
    REQUIRE(0 == pool2.allocFrozenString(2));
  }

  SECTION("Returns same address after clear()") {
    void *a = pool.allocFrozenString(1);
    pool.clear();
    void *b = pool.allocFrozenString(1);

    REQUIRE(a == b);
  }

  SECTION("Can use full capacity when fresh") {
    void *a = pool.allocFrozenString(longestString);

    REQUIRE(a != 0);
  }

  SECTION("Can use full capacity after clear") {
    pool.allocFrozenString(longestString);
    pool.clear();

    void *a = pool.allocFrozenString(longestString);

    REQUIRE(a != 0);
  }
}
