// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("JsonArray::remove()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  array.add(1);
  array.add(2);
  array.add(3);

  SECTION("remove first by index") {
    array.remove(0);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 2);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove middle by index") {
    array.remove(1);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove last by index") {
    array.remove(2);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
  }

  SECTION("remove first by iterator") {
    JsonArray::iterator it = array.begin();
    array.remove(it);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 2);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove middle by iterator") {
    JsonArray::iterator it = array.begin();
    ++it;
    array.remove(it);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove last bty iterator") {
    JsonArray::iterator it = array.begin();
    ++it;
    ++it;
    array.remove(it);

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
  }

  SECTION("remove end()") {
    array.remove(array.end());

    REQUIRE(3 == array.size());
  }

  SECTION("In a loop") {
    for (JsonArray::iterator it = array.begin(); it != array.end(); ++it) {
      if (*it == 2)
        array.remove(it);
    }

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }

  SECTION("remove by index on unbound reference") {
    JsonArray unboundArray;
    unboundArray.remove(20);
  }

  SECTION("remove by iterator on unbound reference") {
    JsonArray unboundArray;
    unboundArray.remove(unboundArray.begin());
  }

  SECTION("use JsonVariant as index") {
    array.remove(array[3]);  // no effect with null variant
    array.remove(array[0]);  // remove element at index 1

    REQUIRE(2 == array.size());
    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 3);
  }
}

TEST_CASE("Removed elements are recycled") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonArray array = doc.to<JsonArray>();

  // fill the pool entirely
  for (int i = 0; i < ARDUINOJSON_POOL_CAPACITY; i++)
    array.add(i);

  // free one slot in the pool
  array.remove(0);

  // add one element; it should use the free slot
  array.add(42);

  REQUIRE(spy.log() == AllocatorLog{
                           Allocate(sizeofPool()),  // only one pool
                       });
}
