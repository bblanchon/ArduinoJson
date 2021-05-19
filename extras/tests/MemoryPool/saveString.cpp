// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

static const char *saveString(MemoryPool &pool, const char *s) {
  return pool.saveString(adaptString(const_cast<char *>(s)));
}

TEST_CASE("MemoryPool::saveString()") {
  char buffer[32];
  MemoryPool pool(buffer, 32);

  SECTION("Duplicates different strings") {
    const char *a = saveString(pool, "hello");
    const char *b = saveString(pool, "world");
    REQUIRE(a != b);
    REQUIRE(pool.size() == 6 + 6);
  }

  SECTION("Deduplicates identical strings") {
    const char *a = saveString(pool, "hello");
    const char *b = saveString(pool, "hello");
    REQUIRE(a == b);
    REQUIRE(pool.size() == 6);
  }

  SECTION("Returns NULL when full") {
    REQUIRE(pool.capacity() == 32);

    const void *p1 = saveString(pool, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(p1 != 0);
    REQUIRE(pool.size() == 32);

    const void *p2 = saveString(pool, "b");
    REQUIRE(p2 == 0);
  }

  SECTION("Returns NULL when pool is too small") {
    const void *p = saveString(pool, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    REQUIRE(0 == p);
  }

  SECTION("Returns NULL when buffer is NULL") {
    MemoryPool pool2(0, 32);
    REQUIRE(0 == saveString(pool2, "a"));
  }

  SECTION("Returns NULL when capacity is 0") {
    MemoryPool pool2(buffer, 0);
    REQUIRE(0 == saveString(pool2, "a"));
  }

  SECTION("Returns same address after clear()") {
    const void *a = saveString(pool, "hello");
    pool.clear();
    const void *b = saveString(pool, "world");

    REQUIRE(a == b);
  }

  SECTION("Can use full capacity when fresh") {
    const void *a = saveString(pool, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    REQUIRE(a != 0);
  }

  SECTION("Can use full capacity after clear") {
    saveString(pool, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    pool.clear();

    const void *a = saveString(pool, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    REQUIRE(a != 0);
  }
}
