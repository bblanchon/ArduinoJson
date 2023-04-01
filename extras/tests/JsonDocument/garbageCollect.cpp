// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdlib.h>  // malloc, free
#include <catch.hpp>
#include <utility>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofObject;

TEST_CASE("JsonDocument::garbageCollect()") {
  SpyingAllocator spyingAllocator;
  ControllableAllocator controllableAllocator;
  JsonDocument doc(4096, &controllableAllocator);

  SECTION("when allocation succeeds") {
    deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
    REQUIRE(doc.capacity() == 4096);
    REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
    doc.remove("blanket");

    bool result = doc.garbageCollect();

    REQUIRE(result == true);
    REQUIRE(doc.memoryUsage() == sizeofObject(1) + 8);
    REQUIRE(doc.capacity() == 4096);
    REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
  }

  SECTION("when allocation fails") {
    deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
    REQUIRE(doc.capacity() == 4096);
    REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
    doc.remove("blanket");
    controllableAllocator.disable();

    bool result = doc.garbageCollect();

    REQUIRE(result == false);
    REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
    REQUIRE(doc.capacity() == 4096);
    REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
  }
}
