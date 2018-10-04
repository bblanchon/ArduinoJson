// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

typedef DynamicMemoryPool::StringBuilder StringBuilder;

TEST_CASE("DynamicMemoryPool::startString()") {
  SECTION("WorksWhenBufferIsBigEnough") {
    DynamicMemoryPool memoryPool(6);

    StringBuilder str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(str.complete().equals("hello"));
  }

  SECTION("GrowsWhenBufferIsTooSmall") {
    DynamicMemoryPool memoryPool(5);

    StringBuilder str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(str.complete().equals("hello"));
  }

  SECTION("SizeIncreases") {
    DynamicMemoryPool memoryPool(5);

    StringBuilder str = memoryPool.startString();
    REQUIRE(0 == memoryPool.size());

    str.append('h');
    REQUIRE(1 == memoryPool.size());

    str.complete();
    REQUIRE(2 == memoryPool.size());
  }
}
