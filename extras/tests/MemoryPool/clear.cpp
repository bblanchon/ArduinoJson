// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

static const size_t poolCapacity = 512;

TEST_CASE("MemoryPool::clear()") {
  char buffer[poolCapacity];
  MemoryPool pool(buffer, sizeof(buffer));

  SECTION("Discards allocated variants") {
    pool.allocVariant();

    pool.clear();
    REQUIRE(pool.size() == 0);
  }

  SECTION("Discards allocated strings") {
    pool.allocFrozenString(10);
    REQUIRE(pool.size() > 0);

    pool.clear();

    REQUIRE(pool.size() == 0);
  }
}
