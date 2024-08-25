// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::allocVariant()") {
  SECTION("Returns different pointer") {
    ResourceManager resources;

    auto s1 = resources.allocVariant();
    REQUIRE(s1.ptr() != nullptr);
    auto s2 = resources.allocVariant();
    REQUIRE(s2.ptr() != nullptr);

    REQUIRE(s1.ptr() != s2.ptr());
  }

  SECTION("Returns the same slot after calling freeVariant()") {
    ResourceManager resources;

    auto s1 = resources.allocVariant();
    auto s2 = resources.allocVariant();
    resources.freeVariant(s1);
    resources.freeVariant(s2);
    auto s3 = resources.allocVariant();
    auto s4 = resources.allocVariant();
    auto s5 = resources.allocVariant();

    REQUIRE(s2.id() != s1.id());
    REQUIRE(s3.id() == s2.id());
    REQUIRE(s4.id() == s1.id());
    REQUIRE(s5.id() != s1.id());
    REQUIRE(s5.id() != s2.id());
  }

  SECTION("Returns aligned pointers") {
    ResourceManager resources;

    REQUIRE(isAligned(resources.allocVariant().ptr()));
    REQUIRE(isAligned(resources.allocVariant().ptr()));
  }

  SECTION("Returns null if pool list allocation fails") {
    ResourceManager resources(FailingAllocator::instance());

    auto variant = resources.allocVariant();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(variant.ptr() == nullptr);
  }

  SECTION("Returns null if pool allocation fails") {
    ResourceManager resources(FailingAllocator::instance());

    resources.allocVariant();

    auto variant = resources.allocVariant();
    REQUIRE(variant.id() == NULL_SLOT);
    REQUIRE(variant.ptr() == nullptr);
  }

  SECTION("Try overflow pool counter") {
    ResourceManager resources;

    // this test assumes SlotId is 8-bit; otherwise it consumes a lot of memory
    // tyhe GitHub Workflow gets killed
    REQUIRE(NULL_SLOT == 255);

    // fill all the pools
    for (SlotId i = 0; i < NULL_SLOT; i++) {
      auto slot = resources.allocVariant();
      REQUIRE(slot.id() == i);  // or != NULL_SLOT
      REQUIRE(slot.ptr() != nullptr);
    }

    REQUIRE(resources.overflowed() == false);

    // now all allocations should fail
    for (int i = 0; i < 10; i++) {
      auto slot = resources.allocVariant();
      REQUIRE(slot.id() == NULL_SLOT);
      REQUIRE(slot.ptr() == nullptr);
    }

    REQUIRE(resources.overflowed() == true);
  }
}
