// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>
#include <sstream>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("DynamicMemoryPool::allocFrozenString()") {
  DynamicMemoryPool pool;

  SECTION("Returns different pointers") {
    StringSlot* a = pool.allocFrozenString(1);
    StringSlot* b = pool.allocFrozenString(2);
    REQUIRE(a != b);
    REQUIRE(a->value != b->value);
  }

  SECTION("Returns same slot after freeString") {
    StringSlot* a = pool.allocFrozenString(1);
    pool.freeString(a);
    StringSlot* b = pool.allocFrozenString(2);
    REQUIRE(a == b);
    REQUIRE(a->value == b->value);
  }
}
