// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

static const size_t poolCapacity = 512;

TEST_CASE("StaticMemoryPool::clear()") {
  StaticMemoryPool<poolCapacity> memoryPool;

  SECTION("Discards allocated variants") {
    memoryPool.allocVariant();

    memoryPool.clear();
    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Discards allocated strings") {
    memoryPool.allocFrozenString(10);

    memoryPool.clear();

    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Purges variant cache") {
    VariantSlot* a = memoryPool.allocVariant();
    REQUIRE(a != 0);
    VariantSlot* b = memoryPool.allocVariant();
    REQUIRE(b != 0);

    // place slot a in the pool of free slots
    memoryPool.freeVariant(a);
    memoryPool.clear();

    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Purges string cache") {
    StringSlot* a = memoryPool.allocFrozenString(10);
    REQUIRE(a != 0);
    StringSlot* b = memoryPool.allocFrozenString(10);
    REQUIRE(b != 0);

    // place slot a in the pool of free slots
    memoryPool.freeString(a);
    memoryPool.clear();

    REQUIRE(memoryPool.size() == 0);
  }

  SECTION("Purges list of string") {
    StringSlot* a = memoryPool.allocFrozenString(6);
    REQUIRE(a != 0);
    strcpy(a->value, "hello");

    StringSlot* b = memoryPool.allocFrozenString(6);
    REQUIRE(b != 0);
    strcpy(b->value, "world");

    memoryPool.clear();  // ACT!

    StringSlot* c = memoryPool.allocFrozenString(2);
    REQUIRE(c != 0);
    strcpy(c->value, "H");

    StringSlot* d = memoryPool.allocFrozenString(2);
    REQUIRE(d != 0);
    strcpy(d->value, "W");

    // if the memory pool keeps pointer to the old strings
    // it will try to compact the strings
    memoryPool.freeString(c);

    REQUIRE(d->value == std::string("W"));
  }
}
