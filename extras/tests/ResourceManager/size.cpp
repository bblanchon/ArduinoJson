// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/VariantPoolImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::size()") {
  TimebombAllocator timebomb(0);
  ResourceManager resources(&timebomb);

  SECTION("Initial size is 0") {
    REQUIRE(0 == resources.size());
  }

  SECTION("Doesn't grow when allocation of second pool fails") {
    timebomb.setCountdown(1);
    for (size_t i = 0; i < ARDUINOJSON_POOL_CAPACITY; i++)
      resources.allocSlot();
    size_t size = resources.size();

    resources.allocSlot();

    REQUIRE(size == resources.size());
  }
}
