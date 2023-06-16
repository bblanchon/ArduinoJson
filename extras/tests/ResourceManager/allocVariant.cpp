// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::allocVariant()") {
  SECTION("Returns different pointer") {
    ResourceManager resources(4096);

    VariantSlot* s1 = resources.allocVariant();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = resources.allocVariant();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns aligned pointers") {
    ResourceManager resources(4096);

    REQUIRE(isAligned(resources.allocVariant()));
    REQUIRE(isAligned(resources.allocVariant()));
  }

  SECTION("Returns zero if capacity is 0") {
    ResourceManager resources(0);

    REQUIRE(resources.allocVariant() == 0);
  }

  SECTION("Returns zero if buffer is null") {
    ResourceManager resources(4096, FailingAllocator::instance());

    REQUIRE(resources.allocVariant() == 0);
  }

  SECTION("Returns zero if capacity is insufficient") {
    ResourceManager resources(sizeof(VariantSlot));

    resources.allocVariant();

    REQUIRE(resources.allocVariant() == 0);
  }
}
