// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Memory/StringBuilder.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

static char buffer[4096];

TEST_CASE("StringBuilder") {
  SECTION("Works when buffer is big enough") {
    MemoryPool memoryPool(buffer, addPadding(JSON_STRING_SIZE(6)));

    StringBuilder str(&memoryPool);
    str.append("hello");

    REQUIRE(str.complete().equals("hello"));
  }

  SECTION("Returns null when too small") {
    MemoryPool memoryPool(buffer, sizeof(void*));

    StringBuilder str(&memoryPool);
    str.append("hello world!");

    REQUIRE(str.complete().isNull());
  }

  SECTION("Increases size of memory pool") {
    MemoryPool memoryPool(buffer, addPadding(JSON_STRING_SIZE(6)));

    StringBuilder str(&memoryPool);
    str.append('h');
    str.complete();

    REQUIRE(JSON_STRING_SIZE(2) == memoryPool.size());
  }
}
