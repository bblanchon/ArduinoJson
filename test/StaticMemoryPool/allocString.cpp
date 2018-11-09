// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StaticMemoryPool::allocFrozenString()") {
  const size_t poolCapacity = 64;
  const size_t longestString = poolCapacity - sizeof(StringSlot);
  StaticMemoryPool<poolCapacity> pool;

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

  SECTION("Returns same address after freeString()") {
    StringSlot *a = pool.allocFrozenString(1);
    pool.freeString(a);
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

TEST_CASE("StaticMemoryPool::freeString()") {
  const size_t poolCapacity = 512;
  const size_t longestString = poolCapacity - sizeof(StringSlot);
  StaticMemoryPool<poolCapacity> pool;

  static const size_t testStringSize =
      (poolCapacity - sizeof(StringSlot) * 4 - sizeof(VariantSlot) * 4) / 4;

  SECTION("Restores full capacity") {
    StringSlot *strings[4];
    VariantSlot *variants[4];

    for (int i = 0; i < 4; i++) {
      strings[i] = pool.allocFrozenString(testStringSize);
      REQUIRE(strings[i] != 0);
      variants[i] = pool.allocVariant();
      REQUIRE(variants[i] != 0);
    }

    // In random order
    pool.freeString(strings[2]);
    pool.freeVariant(variants[3]);
    pool.freeVariant(variants[0]);
    pool.freeString(strings[0]);
    pool.freeVariant(variants[1]);
    pool.freeString(strings[1]);
    pool.freeVariant(variants[2]);
    pool.freeString(strings[3]);

    StringSlot *b = pool.allocFrozenString(longestString);

    REQUIRE(b != 0);
    REQUIRE(b->size == longestString);
  }

  SECTION("Move strings") {
    StringSlot *a = pool.allocFrozenString(6);
    strcpy(a->value, "hello");

    StringSlot *b = pool.allocFrozenString(7);
    strcpy(b->value, "world!");
    pool.freeString(a);

    REQUIRE(b->size == 7);
    REQUIRE(b->value == std::string("world!"));
    REQUIRE(a->value == b->value);
  }

  SECTION("Accepts non-frozen string") {
    StringSlot *a = pool.allocExpandableString();
    pool.freeString(a);

    REQUIRE(pool.size() == 0);
  }
}
