// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

static StringNode* saveString(MemoryPool& pool, const char* s) {
  return pool.saveString(adaptString(s));
}

static StringNode* saveString(MemoryPool& pool, const char* s, size_t n) {
  return pool.saveString(adaptString(s, n));
}

TEST_CASE("MemoryPool::saveString()") {
  MemoryPool pool(32);

  SECTION("Duplicates different strings") {
    auto a = saveString(pool, "hello");
    auto b = saveString(pool, "world");
    REQUIRE(a->data != b->data);
    REQUIRE(a->length == 5);
    REQUIRE(b->length == 5);
    REQUIRE(a->references == 1);
    REQUIRE(b->references == 1);
    REQUIRE(pool.size() == 2 * sizeofString(5));
  }

  SECTION("Deduplicates identical strings") {
    auto a = saveString(pool, "hello");
    auto b = saveString(pool, "hello");
    REQUIRE(a == b);
    REQUIRE(a->length == 5);
    REQUIRE(a->references == 2);
    REQUIRE(pool.size() == sizeofString(5));
  }

  SECTION("Deduplicates identical strings that contain NUL") {
    auto a = saveString(pool, "hello\0world", 11);
    auto b = saveString(pool, "hello\0world", 11);
    REQUIRE(a == b);
    REQUIRE(a->length == 11);
    REQUIRE(a->references == 2);
    REQUIRE(pool.size() == sizeofString(11));
  }

  SECTION("Don't stop on first NUL") {
    auto a = saveString(pool, "hello");
    auto b = saveString(pool, "hello\0world", 11);
    REQUIRE(a != b);
    REQUIRE(a->length == 5);
    REQUIRE(b->length == 11);
    REQUIRE(a->references == 1);
    REQUIRE(b->references == 1);
    REQUIRE(pool.size() == sizeofString(5) + sizeofString(11));
  }

  SECTION("Returns NULL when allocation fails") {
    MemoryPool pool2(32, FailingAllocator::instance());
    REQUIRE(saveString(pool2, "a") == nullptr);
  }
}
