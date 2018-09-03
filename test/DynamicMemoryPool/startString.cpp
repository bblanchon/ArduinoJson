// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("DynamicMemoryPool::startString()") {
  SECTION("WorksWhenBufferIsBigEnough") {
    DynamicMemoryPool memoryPool(6);

    DynamicMemoryPool::String str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(std::string("hello") == str.c_str());
  }

  SECTION("GrowsWhenBufferIsTooSmall") {
    DynamicMemoryPool memoryPool(5);

    DynamicMemoryPool::String str = memoryPool.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(std::string("hello") == str.c_str());
  }

  SECTION("SizeIncreases") {
    DynamicMemoryPool memoryPool(5);

    DynamicMemoryPool::String str = memoryPool.startString();
    REQUIRE(0 == memoryPool.size());

    str.append('h');
    REQUIRE(1 == memoryPool.size());

    str.c_str();
    REQUIRE(2 == memoryPool.size());
  }
}
