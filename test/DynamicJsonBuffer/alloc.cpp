// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <sstream>

using namespace ArduinoJson::Internals;

static bool isAligned(void* ptr) {
  const size_t mask = sizeof(void*) - 1;
  size_t addr = reinterpret_cast<size_t>(ptr);
  return (addr & mask) == 0;
}

std::stringstream allocatorLog;

struct SpyingAllocator : DefaultAllocator {
  void* allocate(size_t n) {
    allocatorLog << "A" << (n - DynamicJsonBuffer::EmptyBlockSize);
    return DefaultAllocator::allocate(n);
  }
  void deallocate(void* p) {
    allocatorLog << "F";
    return DefaultAllocator::deallocate(p);
  }
};

TEST_CASE("DynamicJsonBuffer::alloc()") {
  SECTION("Returns different pointers") {
    DynamicJsonBuffer buffer;
    void* p1 = buffer.alloc(1);
    void* p2 = buffer.alloc(2);
    REQUIRE(p1 != p2);
  }

  SECTION("Doubles allocation size when full") {
    allocatorLog.str("");
    {
      DynamicJsonBufferBase<SpyingAllocator> buffer(1);
      buffer.alloc(1);
      buffer.alloc(1);
    }
    REQUIRE(allocatorLog.str() == "A1A2FF");
  }

  SECTION("Resets allocation size after clear()") {
    allocatorLog.str("");
    {
      DynamicJsonBufferBase<SpyingAllocator> buffer(1);
      buffer.alloc(1);
      buffer.alloc(1);
      buffer.clear();
      buffer.alloc(1);
    }
    REQUIRE(allocatorLog.str() == "A1A2FFA1F");
  }

  SECTION("Makes a big allocation when needed") {
    allocatorLog.str("");
    {
      DynamicJsonBufferBase<SpyingAllocator> buffer(1);
      buffer.alloc(42);
    }
    REQUIRE(allocatorLog.str() == "A42F");
  }

  SECTION("Alignment") {
    // make room for two but not three
    DynamicJsonBuffer tinyBuf(2 * sizeof(void*) + 1);

    REQUIRE(isAligned(tinyBuf.alloc(1)));  // this on is aligned by design
    REQUIRE(isAligned(tinyBuf.alloc(1)));  // this one fits in the first block
    REQUIRE(isAligned(tinyBuf.alloc(1)));  // this one requires a new block
  }
}
