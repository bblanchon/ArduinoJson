// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/StaticMemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("StringBuilder") {
  SECTION("WorksWhenBufferIsBigEnough") {
    StaticMemoryPool<JSON_STRING_SIZE(6)> memoryPool;

    StringBuilder str(&memoryPool);
    str.append("hello");

    REQUIRE(str.complete().equals("hello"));
  }

  SECTION("ReturnsNullWhenTooSmall") {
    StaticMemoryPool<1> memoryPool;

    StringBuilder str(&memoryPool);
    str.append("hello!!!");

    REQUIRE(str.complete().isNull());
  }

  SECTION("Increases size of memory pool") {
    StaticMemoryPool<JSON_STRING_SIZE(6)> memoryPool;

    StringBuilder str(&memoryPool);
    str.append('h');
    str.complete();

    REQUIRE(JSON_STRING_SIZE(2) == memoryPool.size());
  }
}
