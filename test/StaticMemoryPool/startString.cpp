// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("StaticMemoryPool::startString()") {
  SECTION("WorksWhenBufferIsBigEnough") {
    StaticMemoryPool<6> memoryPool;

    StaticMemoryPoolBase::String str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(std::string("hello") == str.c_str());
  }

  SECTION("ReturnsNullWhenTooSmall") {
    StaticMemoryPool<5> memoryPool;

    StaticMemoryPoolBase::String str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(0 == str.c_str());
  }

  SECTION("SizeIncreases") {
    StaticMemoryPool<5> memoryPool;

    StaticMemoryPoolBase::String str = memoryPool.startString();
    REQUIRE(0 == memoryPool.size());

    str.append('h');
    REQUIRE(1 == memoryPool.size());

    str.c_str();
    REQUIRE(2 == memoryPool.size());
  }
}
