// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

static const size_t poolCapacity = 512;

TEST_CASE("MemoryPool::clear()") {
  MemoryPool pool(poolCapacity);

  SECTION("Discards allocated variants") {
    pool.allocVariant();

    pool.clear();
    REQUIRE(pool.size() == 0);
  }

  SECTION("Discards allocated strings") {
    pool.saveString(adaptString("123456789"));
    REQUIRE(pool.size() == sizeofString(9));

    pool.clear();

    REQUIRE(pool.size() == 0);
  }
}
