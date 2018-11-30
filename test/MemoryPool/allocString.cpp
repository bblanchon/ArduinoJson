// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("MemoryPool::allocFrozenString()") {
  const size_t poolCapacity = 64;
  const size_t longestString = poolCapacity - sizeof(StringSlot);
  char buffer[poolCapacity];
  MemoryPool pool(buffer, poolCapacity);

  SECTION("Returns different addresses") {
    StringSlot *a = pool.allocFrozenString(1);
    StringSlot *b = pool.allocFrozenString(1);
    REQUIRE(a != b);
    REQUIRE(a->value != b->value);
  }

  SECTION("Returns a StringSlot of the right size") {
    StringSlot *s = pool.allocFrozenString(12);
    REQUIRE(s->size == 12);
  }

  SECTION("Returns NULL when full") {
    pool.allocFrozenString(longestString);
    void *p = pool.allocFrozenString(1);
    REQUIRE(0 == p);
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

  SECTION("Returns aligned pointers") {
    REQUIRE(isAligned(pool.allocFrozenString(1)));
    REQUIRE(isAligned(pool.allocFrozenString(1)));
  }

  SECTION("Returns same address after clear()") {
    StringSlot *a = pool.allocFrozenString(1);
    pool.clear();
    StringSlot *b = pool.allocFrozenString(1);

    REQUIRE(a == b);
    REQUIRE(a->value == b->value);
  }

  SECTION("Can use full capacity when fresh") {
    StringSlot *a = pool.allocFrozenString(longestString);

    REQUIRE(a != 0);
  }

  SECTION("Can use full capacity after clear") {
    pool.allocFrozenString(longestString);
    pool.clear();

    StringSlot *a = pool.allocFrozenString(longestString);

    REQUIRE(a != 0);
  }
}
