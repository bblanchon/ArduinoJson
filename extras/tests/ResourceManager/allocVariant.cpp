// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/VariantPoolImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::allocSlot()") {
  SECTION("Returns different pointer") {
    ResourceManager resources(4096);

    VariantSlot* s1 = resources.allocSlot();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = resources.allocSlot();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns aligned pointers") {
    ResourceManager resources(4096);

    REQUIRE(isAligned(resources.allocSlot().operator VariantSlot*()));
    REQUIRE(isAligned(resources.allocSlot().operator VariantSlot*()));
  }

  SECTION("Returns zero if capacity is 0") {
    ResourceManager resources(0);

    auto variant = resources.allocSlot();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(static_cast<VariantSlot*>(variant) == nullptr);
  }

  SECTION("Returns zero if buffer is null") {
    ResourceManager resources(4096, FailingAllocator::instance());

    auto variant = resources.allocSlot();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(static_cast<VariantSlot*>(variant) == nullptr);
  }

  SECTION("Returns zero if capacity is insufficient") {
    ResourceManager resources(sizeof(VariantSlot));

    resources.allocSlot();

    auto variant = resources.allocSlot();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(static_cast<VariantSlot*>(variant) == nullptr);
  }
}
