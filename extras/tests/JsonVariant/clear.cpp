// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <stdint.h>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonVariant::clear()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonVariant var = doc.to<JsonVariant>();

  SUBCASE("size goes back to zero") {
    var.add(42);
    var.clear();

    REQUIRE(var.size() == 0);
  }

  SUBCASE("isNull() return true") {
    var.add("hello");
    var.clear();

    REQUIRE(var.isNull() == true);
  }

  SUBCASE("releases owned string") {
    var.set("hello"_s);
    var.clear();

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                             Deallocate(sizeofString("hello")),
                         });
  }
}
