// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;
using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonDocument assignment") {
  SpyingAllocator spyingAllocator;

  SECTION("Copy assignment same capacity") {
    JsonDocument doc1(1024, &spyingAllocator);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    JsonDocument doc2(1024, &spyingAllocator);
    spyingAllocator.clearLog();

    doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(5))  // hello
                           << AllocatorLog::Allocate(sizeofString(5))  // world
    );
  }

  SECTION("Copy assignment reallocates when capacity is smaller") {
    const size_t capacity = 100 * sizeof(ArduinoJson::detail::VariantSlot);
    JsonDocument doc1(capacity, &spyingAllocator);
    deserializeJson(doc1, "[{\"hello\":\"world\"}]");
    JsonDocument doc2(sizeofArray(1), &spyingAllocator);
    spyingAllocator.clearLog();

    doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "[{\"hello\":\"world\"}]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Deallocate(sizeofArray(1))
                           << AllocatorLog::Allocate(capacity)
                           << AllocatorLog::Allocate(sizeofString(5))  // hello
                           << AllocatorLog::Allocate(sizeofString(5))  // world
    );
  }

  SECTION("Copy assignment reallocates when capacity is larger") {
    const size_t capacity1 = 100 * sizeof(ArduinoJson::detail::VariantSlot);
    JsonDocument doc1(capacity1, &spyingAllocator);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    JsonDocument doc2(4096, &spyingAllocator);
    spyingAllocator.clearLog();

    doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Deallocate(4096)
                           << AllocatorLog::Allocate(capacity1)
                           << AllocatorLog::Allocate(sizeofString(5))  // hello
                           << AllocatorLog::Allocate(sizeofString(5))  // world
    );
  }

  SECTION("Move assign") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      JsonDocument doc2(128, &spyingAllocator);

      doc2 = std::move(doc1);

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
    }
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Allocate(128)
                           << AllocatorLog::Deallocate(128)
                           << AllocatorLog::Deallocate(sizeofString(31))
                           << AllocatorLog::Deallocate(4096));
  }

  SECTION("Assign from JsonObject") {
    JsonDocument doc1(200);
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(4096);
    doc2 = obj;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("Assign from JsonArray") {
    JsonDocument doc1(200);
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(4096);
    doc2 = arr;

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
  }

  SECTION("Assign from JsonVariant") {
    JsonDocument doc1(200);
    deserializeJson(doc1, "42");

    JsonDocument doc2(4096);
    doc2 = doc1.as<JsonVariant>();

    REQUIRE(doc2.as<std::string>() == "42");
  }

  SECTION("Assign from MemberProxy") {
    JsonDocument doc1(200);
    doc1["value"] = 42;

    JsonDocument doc2(4096);
    doc2 = doc1["value"];

    REQUIRE(doc2.as<std::string>() == "42");
  }

  SECTION("Assign from ElementProxy") {
    JsonDocument doc1(200);
    doc1[0] = 42;

    JsonDocument doc2(4096);
    doc2 = doc1[0];

    REQUIRE(doc2.as<std::string>() == "42");
  }
}
