// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/DynamicMemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("DynamicMemoryPool::startString()") {
  SECTION("WorksWhenBufferIsBigEnough") {
    DynamicMemoryPool memoryPool(JSON_STRING_SIZE(8));

    StringBuilder str(&memoryPool);
    str.append("abcdefg");

    REQUIRE(memoryPool.blockCount() == 1);
    REQUIRE(str.complete().equals("abcdefg"));
  }

  SECTION("GrowsWhenBufferIsTooSmall") {
    DynamicMemoryPool memoryPool(JSON_STRING_SIZE(8));

    StringBuilder str(&memoryPool);
    str.append("abcdefghABC");

    REQUIRE(memoryPool.blockCount() == 2);
    REQUIRE(str.complete().equals("abcdefghABC"));
  }

  SECTION("SizeIncreases") {
    DynamicMemoryPool memoryPool(JSON_STRING_SIZE(5));

    StringBuilder str(&memoryPool);
    str.append('h');
    str.complete();

    REQUIRE(JSON_STRING_SIZE(2) == memoryPool.size());
  }
}
