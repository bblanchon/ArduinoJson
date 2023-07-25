// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonVariant::clear()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("size goes back to zero") {
    var.add(42);
    var.clear();

    REQUIRE(var.size() == 0);
  }

  SECTION("isNull() return true") {
    var.add("hello");
    var.clear();

    REQUIRE(var.isNull() == true);
  }

  SECTION("releases owned string") {
    var.set(std::string("hello"));
    var.clear();

    REQUIRE(spy.log() == AllocatorLog()
                             << AllocatorLog::Allocate(sizeofString(5))
                             << AllocatorLog::Deallocate(sizeofString(5)));
  }
}
