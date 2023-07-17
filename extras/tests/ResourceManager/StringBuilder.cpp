// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("StringBuilder") {
  ControllableAllocator controllableAllocator;
  SpyingAllocator spyingAllocator(&controllableAllocator);
  ResourceManager resources(&spyingAllocator);

  SECTION("Empty string") {
    StringBuilder str(&resources);

    str.startString();
    str.save();

    REQUIRE(resources.size() == sizeofString(0));
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Reallocate(sizeofString(31),
                                                       sizeofString(0)));
  }

  SECTION("Short string fits in first allocation") {
    StringBuilder str(&resources);

    str.startString();
    str.append("hello");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "hello");
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31)));
  }

  SECTION("Long string needs reallocation") {
    StringBuilder str(&resources);

    str.startString();
    str.append(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() ==
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
            "eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Reallocate(sizeofString(31),
                                                       sizeofString(63))
                           << AllocatorLog::Reallocate(sizeofString(63),
                                                       sizeofString(127)));
  }

  SECTION("Realloc fails") {
    StringBuilder str(&resources);

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
    REQUIRE(resources.overflowed() == true);
  }

  SECTION("Initial allocation fails") {
    StringBuilder str(&resources);

    controllableAllocator.disable();
    str.startString();

    REQUIRE(str.isValid() == false);
    REQUIRE(resources.overflowed() == true);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::AllocateFail(sizeofString(31)));
  }
}

static StringNode* addStringToPool(ResourceManager& resources, const char* s) {
  StringBuilder str(&resources);
  str.startString();
  str.append(s);
  return str.save();
}

TEST_CASE("StringBuilder::save() deduplicates strings") {
  ResourceManager resources;

  SECTION("Basic") {
    auto s1 = addStringToPool(resources, "hello");
    auto s2 = addStringToPool(resources, "world");
    auto s3 = addStringToPool(resources, "hello");

    REQUIRE(s1 == s3);
    REQUIRE(s2 != s3);
    REQUIRE(s1->references == 2);
    REQUIRE(s2->references == 1);
    REQUIRE(s3->references == 2);
    REQUIRE(resources.size() == 2 * sizeofString(5));
  }

  SECTION("Requires terminator") {
    auto s1 = addStringToPool(resources, "hello world");
    auto s2 = addStringToPool(resources, "hello");

    REQUIRE(s2 != s1);
    REQUIRE(s1->references == 1);
    REQUIRE(s2->references == 1);
    REQUIRE(resources.size() == sizeofString(11) + sizeofString(5));
  }

  SECTION("Don't overrun") {
    auto s1 = addStringToPool(resources, "hello world");
    auto s2 = addStringToPool(resources, "wor");

    REQUIRE(s2 != s1);
    REQUIRE(s1->references == 1);
    REQUIRE(s2->references == 1);
    REQUIRE(resources.size() == sizeofString(11) + sizeofString(3));
  }
}
