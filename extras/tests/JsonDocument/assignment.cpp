// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

TEST_CASE("JsonDocument assignment") {
  SpyingAllocator spyingAllocator;

  SECTION("Copy assignment same capacity") {
    {
      JsonDocument doc1(1024, &spyingAllocator);
      deserializeJson(doc1, "{\"hello\":\"world\"}");
      JsonDocument doc2(1024, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
      REQUIRE(doc2.capacity() == doc1.capacity());
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Deallocate(1024)
                                         << AllocatorLog::Deallocate(1024));
  }

  SECTION("Copy assignment reallocates when capacity is smaller") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      deserializeJson(doc1, "{\"hello\":\"world\"}");
      JsonDocument doc2(8, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
      REQUIRE(doc2.capacity() == doc1.capacity());
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Allocate(8)
                                         << AllocatorLog::Deallocate(8)
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate(4096)
                                         << AllocatorLog::Deallocate(4096));
  }

  SECTION("Copy assignment reallocates when capacity is larger") {
    {
      JsonDocument doc1(1024, &spyingAllocator);
      deserializeJson(doc1, "{\"hello\":\"world\"}");
      JsonDocument doc2(4096, &spyingAllocator);

      doc2 = doc1;

      REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
      REQUIRE(doc2.capacity() == doc1.capacity());
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate(4096)
                                         << AllocatorLog::Allocate(1024)
                                         << AllocatorLog::Deallocate(1024)
                                         << AllocatorLog::Deallocate(1024));
  }

  SECTION("Move assign") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));
      JsonDocument doc2(8, &spyingAllocator);

      doc2 = std::move(doc1);

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
      REQUIRE(doc1.capacity() == 0);
      REQUIRE(doc2.capacity() == 4096);
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Allocate(8)
                                         << AllocatorLog::Deallocate(8)
                                         << AllocatorLog::Deallocate(4096));
  }

  SECTION("Assign from JsonObject") {
    JsonDocument doc1(200);
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(4096);
    doc2 = obj;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from JsonArray") {
    JsonDocument doc1(200);
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(4096);
    doc2 = arr;

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from JsonVariant") {
    JsonDocument doc1(200);
    deserializeJson(doc1, "42");

    JsonDocument doc2(4096);
    doc2 = doc1.as<JsonVariant>();

    REQUIRE(doc2.as<std::string>() == "42");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from MemberProxy") {
    JsonDocument doc1(200);
    doc1["value"] = 42;

    JsonDocument doc2(4096);
    doc2 = doc1["value"];

    REQUIRE(doc2.as<std::string>() == "42");
    REQUIRE(doc2.capacity() == 4096);
  }

  SECTION("Assign from ElementProxy") {
    JsonDocument doc1(200);
    doc1[0] = 42;

    JsonDocument doc2(4096);
    doc2 = doc1[0];

    REQUIRE(doc2.as<std::string>() == "42");
    REQUIRE(doc2.capacity() == 4096);
  }
}
