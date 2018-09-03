// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

static bool isAligned(void *ptr) {
  const size_t mask = sizeof(void *) - 1;
  size_t addr = reinterpret_cast<size_t>(ptr);
  return (addr & mask) == 0;
}

TEST_CASE("StaticMemoryPool::alloc()") {
  StaticMemoryPool<64> memoryPool;

  SECTION("Returns different addresses") {
    void *p1 = memoryPool.alloc(1);
    void *p2 = memoryPool.alloc(1);
    REQUIRE(p1 != p2);
  }

  SECTION("Returns non-NULL when using full capacity") {
    void *p = memoryPool.alloc(64);
    REQUIRE(0 != p);
  }

  SECTION("Returns NULL when full") {
    memoryPool.alloc(64);
    void *p = memoryPool.alloc(1);
    REQUIRE(0 == p);
  }

  SECTION("Returns NULL when memoryPool is too small") {
    void *p = memoryPool.alloc(65);
    REQUIRE(0 == p);
  }

  SECTION("Returns aligned pointers") {
    for (size_t size = 1; size <= sizeof(void *); size++) {
      void *p = memoryPool.alloc(1);
      REQUIRE(isAligned(p));
    }
  }

  SECTION("Returns same address after clear()") {
    void *p1 = memoryPool.alloc(1);
    memoryPool.clear();
    void *p2 = memoryPool.alloc(1);
    REQUIRE(p1 == p2);
  }
}
