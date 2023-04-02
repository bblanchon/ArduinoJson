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
  ControllableAllocator controllableAllocator;
  SpyingAllocator spyingAllocator(&controllableAllocator);
  JsonDocument doc(4096, &spyingAllocator);

  SECTION("when allocation succeeds") {
    deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
    REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
    doc.remove("blanket");

    bool result = doc.garbageCollect();

    REQUIRE(result == true);
    REQUIRE(doc.memoryUsage() == sizeofObject(1) + 8);
    REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate(4096));
  }

  SECTION("when allocation fails") {
    deserializeJson(doc, "{\"blanket\":1,\"dancing\":2}");
    REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
    doc.remove("blanket");
    controllableAllocator.disable();

    bool result = doc.garbageCollect();

    REQUIRE(result == false);
    REQUIRE(doc.memoryUsage() == sizeofObject(2) + 16);
    REQUIRE(doc.as<std::string>() == "{\"dancing\":2}");

    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::AllocateFail(4096));
  }
}
