// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StaticMemoryPool::startString()") {
  SECTION("WorksWhenBufferIsBigEnough") {
    StaticMemoryPool<6> memoryPool;

    StringBuilder str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(str.complete().equals("hello"));
  }

  SECTION("ReturnsNullWhenTooSmall") {
    StaticMemoryPool<5> memoryPool;

    StringBuilder str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(str.complete().isNull());
  }

  SECTION("SizeIncreases") {
    StaticMemoryPool<5> memoryPool;

    StringBuilder str = memoryPool.startString();
    str.append('h');
    str.complete();

    REQUIRE(2 == memoryPool.size());
  }
}
