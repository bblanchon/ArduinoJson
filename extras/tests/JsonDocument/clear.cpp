// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include <stdlib.h>  // malloc, free
#include <string>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument::clear()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SUBCASE("null") {
    doc.clear();

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SUBCASE("releases resources") {
    doc["hello"_s] = "world"_s;
    spy.clearLog();

    doc.clear();

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofPool()),
                             Deallocate(sizeofString("hello")),
                             Deallocate(sizeofString("world")),
                         });
  }

  SUBCASE("clear free list") {  // issue #2034
    JsonObject obj = doc.to<JsonObject>();
    obj["a"] = 1;
    obj.clear();  // puts the slot in the free list

    doc.clear();

    doc["b"] = 2;  // will it pick from the free list?
  }
}
