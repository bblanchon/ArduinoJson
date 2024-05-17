// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::allocSlot()") {
  SECTION("Returns different pointer") {
    ResourceManager resources;

    VariantSlot* s1 = resources.allocSlot();
    REQUIRE(s1 != 0);
    VariantSlot* s2 = resources.allocSlot();
    REQUIRE(s2 != 0);

    REQUIRE(s1 != s2);
  }

  SECTION("Returns the same slot after calling freeSlot()") {
    ResourceManager resources;

    auto s1 = resources.allocSlot();
    auto s2 = resources.allocSlot();
    resources.freeSlot(s1);
    resources.freeSlot(s2);
    auto s3 = resources.allocSlot();
    auto s4 = resources.allocSlot();
    auto s5 = resources.allocSlot();

    REQUIRE(s2.id() != s1.id());
    REQUIRE(s3.id() == s2.id());
    REQUIRE(s4.id() == s1.id());
    REQUIRE(s5.id() != s1.id());
    REQUIRE(s5.id() != s2.id());
  }

  SECTION("Returns aligned pointers") {
    ResourceManager resources;

    REQUIRE(isAligned(resources.allocSlot().operator VariantSlot*()));
    REQUIRE(isAligned(resources.allocSlot().operator VariantSlot*()));
  }

  SECTION("Returns null if pool list allocation fails") {
    ResourceManager resources(FailingAllocator::instance());

    auto variant = resources.allocSlot();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(static_cast<VariantSlot*>(variant) == nullptr);
  }

  SECTION("Returns null if pool allocation fails") {
    ResourceManager resources(FailingAllocator::instance());

    resources.allocSlot();

    auto variant = resources.allocSlot();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(static_cast<VariantSlot*>(variant) == nullptr);
  }

  SECTION("Try overflow pool counter") {
    ResourceManager resources;

    // this test assumes SlotId is 8-bit; otherwise it consumes a lot of memory
    // tyhe GitHub Workflow gets killed
    REQUIRE(NULL_SLOT == 255);

    // fill all the pools
    for (SlotId i = 0; i < NULL_SLOT; i++) {
      auto slot = resources.allocSlot();
      REQUIRE(slot.id() == i);  // or != NULL_SLOT
      REQUIRE(static_cast<VariantSlot*>(slot) != nullptr);
    }

    REQUIRE(resources.overflowed() == false);

    // now all allocations should fail
    for (int i = 0; i < 10; i++) {
      auto slot = resources.allocSlot();
      REQUIRE(slot.id() == NULL_SLOT);
      REQUIRE(static_cast<VariantSlot*>(slot) == nullptr);
    }

    REQUIRE(resources.overflowed() == true);
  }
}
