// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>

#include <doctest.h>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::shrinkToFit()") {
  SpyingAllocator spyingAllocator;
  ResourceManager resources(&spyingAllocator);

  SUBCASE("empty") {
    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SUBCASE("only one pool") {
    resources.allocVariant();

    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Reallocate(sizeofPool(), sizeofPool(1)),
            });
  }

  SUBCASE("more pools than initial count") {
    for (size_t i = 0;
         i < ARDUINOJSON_POOL_CAPACITY * ARDUINOJSON_INITIAL_POOL_COUNT + 1;
         i++)
      resources.allocVariant();
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()) * ARDUINOJSON_INITIAL_POOL_COUNT,
                Allocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2)),
                Allocate(sizeofPool()),
            });

    spyingAllocator.clearLog();
    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Reallocate(sizeofPool(), sizeofPool(1)),
                Reallocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2),
                           sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT + 1)),
            });
  }
}
