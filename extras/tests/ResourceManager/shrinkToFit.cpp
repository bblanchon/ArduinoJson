// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/VariantPoolImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::shrinkToFit()") {
  SpyingAllocator spyingAllocator;
  ResourceManager resources(&spyingAllocator);

  SECTION("empty") {
    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("only one pool") {
    resources.allocSlot();

    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Reallocate(sizeofPool(), sizeof(VariantSlot)),
            });
  }

  SECTION("more pools than initial count") {
    for (size_t i = 0;
         i < ARDUINOJSON_POOL_CAPACITY * ARDUINOJSON_INITIAL_POOL_COUNT + 1;
         i++)
      resources.allocSlot();
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
                Reallocate(sizeofPool(), sizeof(VariantSlot)),
                Reallocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2),
                           sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT + 1)),
            });
  }
}
