// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <stdlib.h>  // malloc, free
#include <string>

#include "Allocators.hpp"

TEST_CASE("JsonDocument::clear()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("null") {
    doc.clear();

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("releases resources") {
    doc[std::string("hello")] = std::string("world");
    spy.clearLog();

    doc.clear();

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofPool()),
                             Deallocate(sizeofString("hello")),
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("clear free list") {  // issue #2034
    JsonObject obj = doc.to<JsonObject>();
    obj["a"] = 1;
    obj.clear();  // puts the slot in the free list

    doc.clear();

    doc["b"] = 2;  // will it pick from the free list?
  }
}
