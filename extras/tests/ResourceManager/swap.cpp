// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/ResourceManagerImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

static void fullPreallocatedPools(ResourceManager& resources) {
  for (int i = 0;
       i < ARDUINOJSON_INITIAL_POOL_COUNT * ARDUINOJSON_POOL_CAPACITY; i++)
    resources.allocVariant();
}

TEST_CASE("ResourceManager::swap()") {
  SECTION("Both using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    ResourceManager b(&spy);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();

    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()) * 2,
                         });
  }

  SECTION("Only left using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    ResourceManager b(&spy);
    fullPreallocatedPools(b);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();
    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()) * (ARDUINOJSON_INITIAL_POOL_COUNT + 1),
                Allocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2)),
                Allocate(sizeofPool()),
            });
  }

  SECTION("Only right using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    fullPreallocatedPools(a);
    ResourceManager b(&spy);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();
    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()) * ARDUINOJSON_INITIAL_POOL_COUNT,
                Allocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2)),
                Allocate(sizeofPool()) * 2,
            });
  }

  SECTION("None is using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    fullPreallocatedPools(a);
    ResourceManager b(&spy);
    fullPreallocatedPools(b);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();

    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));
  }
}
