// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/StringStorage/StringCopier.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("StringCopier") {
  ControllableAllocator controllableAllocator;
  SpyingAllocator spyingAllocator(&controllableAllocator);
  MemoryPool pool(0, &spyingAllocator);

  SECTION("Empty string") {
    StringCopier str(&pool);

    str.startString();
    str.save();

    REQUIRE(pool.size() == sizeofString(0));
    REQUIRE(pool.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Reallocate(sizeofString(31),
                                                       sizeofString(0)));
  }

  SECTION("Short string fits in first allocation") {
    StringCopier str(&pool);

    str.startString();
    str.append("hello");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "hello");
    REQUIRE(pool.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31)));
  }

  SECTION("Long string needs reallocation") {
    StringCopier str(&pool);

    str.startString();
    str.append(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() ==
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
            "eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    REQUIRE(pool.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Reallocate(sizeofString(31),
                                                       sizeofString(63))
                           << AllocatorLog::Reallocate(sizeofString(63),
                                                       sizeofString(127)));
  }

  SECTION("Realloc fails") {
    StringCopier str(&pool);

    str.startString();
    controllableAllocator.disable();
    str.append(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::ReallocateFail(sizeofString(31),
                                                           sizeofString(63))
                           << AllocatorLog::Deallocate(sizeofString(31)));
    REQUIRE(str.isValid() == false);
    REQUIRE(pool.overflowed() == true);
  }

  SECTION("Initial allocation fails") {
    StringCopier str(&pool);

    controllableAllocator.disable();
    str.startString();

    REQUIRE(str.isValid() == false);
    REQUIRE(pool.overflowed() == true);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::AllocateFail(sizeofString(31)));
  }
}

static StringNode* addStringToPool(MemoryPool& pool, const char* s) {
  StringCopier str(&pool);
  str.startString();
  str.append(s);
  return str.save();
}

TEST_CASE("StringCopier::save() deduplicates strings") {
  MemoryPool pool(4096);

  SECTION("Basic") {
    auto s1 = addStringToPool(pool, "hello");
    auto s2 = addStringToPool(pool, "world");
    auto s3 = addStringToPool(pool, "hello");

    REQUIRE(s1 == s3);
    REQUIRE(s2 != s3);
    REQUIRE(s1->references == 2);
    REQUIRE(s2->references == 1);
    REQUIRE(s3->references == 2);
    REQUIRE(pool.size() == 2 * sizeofString(5));
  }

  SECTION("Requires terminator") {
    auto s1 = addStringToPool(pool, "hello world");
    auto s2 = addStringToPool(pool, "hello");

    REQUIRE(s2 != s1);
    REQUIRE(s1->references == 1);
    REQUIRE(s2->references == 1);
    REQUIRE(pool.size() == sizeofString(11) + sizeofString(5));
  }

  SECTION("Don't overrun") {
    auto s1 = addStringToPool(pool, "hello world");
    auto s2 = addStringToPool(pool, "wor");

    REQUIRE(s2 != s1);
    REQUIRE(s1->references == 1);
    REQUIRE(s2->references == 1);
    REQUIRE(pool.size() == sizeofString(11) + sizeofString(3));
  }
}
