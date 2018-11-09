// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("DynamicMemoryPool::size()") {
  DynamicMemoryPool memoryPool;

  SECTION("Initial size is 0") {
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("Increases after allocExpandableString()") {
    StringSlot* a = memoryPool.allocExpandableString();
    memoryPool.freezeString(a, 1);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(1));

    StringSlot* b = memoryPool.allocExpandableString();
    memoryPool.freezeString(b, 1);
    REQUIRE(memoryPool.size() == 2 * JSON_STRING_SIZE(1));
  }

  SECTION("Increases after allocVariant()") {
    memoryPool.allocVariant();
    REQUIRE(sizeof(VariantSlot) == memoryPool.size());

    memoryPool.allocVariant();
    REQUIRE(2 * sizeof(VariantSlot) == memoryPool.size());
  }

  SECTION("Decreases after freeVariant()") {
    VariantSlot* a = memoryPool.allocVariant();
    VariantSlot* b = memoryPool.allocVariant();

    memoryPool.freeVariant(b);
    REQUIRE(sizeof(VariantSlot) == memoryPool.size());

    memoryPool.freeVariant(a);
    REQUIRE(0 == memoryPool.size());
  }

  SECTION("Decreases after freeString()") {
    StringSlot* a = memoryPool.allocFrozenString(5);
    StringSlot* b = memoryPool.allocFrozenString(6);

    memoryPool.freeString(b);
    REQUIRE(memoryPool.size() == JSON_STRING_SIZE(5));

    memoryPool.freeString(a);
    REQUIRE(memoryPool.size() == 0);
  }
}
