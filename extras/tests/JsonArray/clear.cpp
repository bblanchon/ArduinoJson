// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("JsonArray::clear()") {
  SECTION("No-op on null JsonArray") {
    JsonArray array;
    array.clear();
    REQUIRE(array.isNull() == true);
    REQUIRE(array.size() == 0);
  }

  SECTION("Removes all elements") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add(2);
    array.clear();
    REQUIRE(array.size() == 0);
    REQUIRE(array.isNull() == false);
  }

  SECTION("Removed elements are recycled") {
    SpyingAllocator spy;
    JsonDocument doc(&spy);
    JsonArray array = doc.to<JsonArray>();

    // fill the pool entirely
    for (int i = 0; i < ARDUINOJSON_POOL_CAPACITY; i++)
      array.add(i);

    // clear and fill again
    array.clear();
    for (int i = 0; i < ARDUINOJSON_POOL_CAPACITY; i++)
      array.add(i);

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }
}
