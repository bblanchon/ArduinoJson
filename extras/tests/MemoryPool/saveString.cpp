// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

static const char* saveString(MemoryPool& pool, const char* s) {
  return pool.saveString(adaptString(s));
}

static const char* saveString(MemoryPool& pool, const char* s, size_t n) {
  return pool.saveString(adaptString(s, n));
}

TEST_CASE("MemoryPool::saveString()") {
  MemoryPool pool(32);

  SECTION("Duplicates different strings") {
    const char* a = saveString(pool, "hello");
    const char* b = saveString(pool, "world");
    REQUIRE(a != b);
    REQUIRE(pool.size() == 2 * sizeofString(5));
  }

  SECTION("Deduplicates identical strings") {
    const char* a = saveString(pool, "hello");
    const char* b = saveString(pool, "hello");
    REQUIRE(a == b);
    REQUIRE(pool.size() == sizeofString(5));
  }

  SECTION("Deduplicates identical strings that contain NUL") {
    const char* a = saveString(pool, "hello\0world", 11);
    const char* b = saveString(pool, "hello\0world", 11);
    REQUIRE(a == b);
    REQUIRE(pool.size() == sizeofString(11));
  }

  SECTION("Don't stop on first NUL") {
    const char* a = saveString(pool, "hello");
    const char* b = saveString(pool, "hello\0world", 11);
    REQUIRE(a != b);
    REQUIRE(pool.size() == sizeofString(5) + sizeofString(11));
  }

  SECTION("Returns NULL when allocation fails") {
    MemoryPool pool2(32, FailingAllocator::instance());
    REQUIRE(0 == saveString(pool2, "a"));
  }
}
