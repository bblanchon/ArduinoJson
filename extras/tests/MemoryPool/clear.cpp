// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
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
    pool.saveString(adaptString(const_cast<char *>("123456789")));
    REQUIRE(pool.size() == 10);

    pool.clear();

    REQUIRE(pool.size() == 0);
  }
}
