// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("StringBuilder") {
  KillswitchAllocator killswitch;
  SpyingAllocator spyingAllocator(&killswitch);
  ResourceManager resources(&spyingAllocator);

  SECTION("Empty string") {
    StringBuilder str(&resources);

    str.startString();
    str.save();

    REQUIRE(resources.size() == sizeofString(""));
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("")),
            });
  }

  SECTION("Short string fits in first allocation") {
    StringBuilder str(&resources);

    str.startString();
    str.append("hello");

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == "hello");
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                     });
  }

  SECTION("Long string needs reallocation") {
    StringBuilder str(&resources);
    const char* lorem =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.";

    str.startString();
    str.append(lorem);

    REQUIRE(str.isValid() == true);
    REQUIRE(str.str() == lorem);
    REQUIRE(resources.overflowed() == false);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer(1)),
                Reallocate(sizeofStringBuffer(1), sizeofStringBuffer(2)),
                Reallocate(sizeofStringBuffer(2), sizeofStringBuffer(3)),
            });
  }

  SECTION("Realloc fails") {
    StringBuilder str(&resources);

    str.startString();
    killswitch.on();
    str.append(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                ReallocateFail(sizeofStringBuffer(), sizeofStringBuffer(2)),
                Deallocate(sizeofStringBuffer()),
            });
    REQUIRE(str.isValid() == false);
    REQUIRE(resources.overflowed() == true);
  }

  SECTION("Initial allocation fails") {
    StringBuilder str(&resources);

    killswitch.on();
    str.startString();

    REQUIRE(str.isValid() == false);
    REQUIRE(resources.overflowed() == true);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         AllocateFail(sizeofStringBuffer()),
                                     });
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
    REQUIRE(resources.size() == sizeofString("hello") + sizeofString("world"));
  }

  SECTION("Requires terminator") {
    auto s1 = addStringToPool(resources, "hello world");
    auto s2 = addStringToPool(resources, "hello");

    REQUIRE(s2 != s1);
    REQUIRE(s1->references == 1);
    REQUIRE(s2->references == 1);
    REQUIRE(resources.size() ==
            sizeofString("hello world") + sizeofString("hello"));
  }

  SECTION("Don't overrun") {
    auto s1 = addStringToPool(resources, "hello world");
    auto s2 = addStringToPool(resources, "wor");

    REQUIRE(s2 != s1);
    REQUIRE(s1->references == 1);
    REQUIRE(s2->references == 1);
    REQUIRE(resources.size() ==
            sizeofString("hello world") + sizeofString("wor"));
  }
}
