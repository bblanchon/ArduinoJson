// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::capacity()") {
  const size_t capacity = 64;
  ResourceManager resources(capacity);
  REQUIRE(capacity == resources.capacity());
}

TEST_CASE("ResourceManager::size()") {
  ResourceManager resources(4096);

  SECTION("Initial size is 0") {
    REQUIRE(0 == resources.size());
  }

  SECTION("Doesn't grow when memory pool is full") {
    const size_t variantCount = resources.capacity() / sizeof(VariantSlot);

    for (size_t i = 0; i < variantCount; i++)
      new (&resources) VariantSlot();
    size_t size = resources.size();

    new (&resources) VariantSlot();

    REQUIRE(size == resources.size());
  }
}
