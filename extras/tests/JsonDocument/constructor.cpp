// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::addPadding;
using ArduinoJson::detail::sizeofObject;
using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonDocument constructor") {
  SpyingAllocator spyingAllocator;

  SECTION("JsonDocument(size_t)") {
    { JsonDocument doc(4096, &spyingAllocator); }
    REQUIRE(spyingAllocator.log() == AllocatorLog()
                                         << AllocatorLog::Allocate(4096)
                                         << AllocatorLog::Deallocate(4096));
  }

  SECTION("JsonDocument(const JsonDocument&)") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      JsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
    }
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Deallocate(sizeofString(31))
                           << AllocatorLog::Deallocate(4096)
                           << AllocatorLog::Deallocate(sizeofString(31))
                           << AllocatorLog::Deallocate(4096));
  }

  SECTION("JsonDocument(JsonDocument&&)") {
    {
      JsonDocument doc1(4096, &spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      JsonDocument doc2(std::move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
    }
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(4096)
                           << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Deallocate(sizeofString(31))
                           << AllocatorLog::Deallocate(4096));
  }

  SECTION("JsonDocument(JsonObject)") {
    JsonDocument doc1(200);
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(obj, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofObject(1)));
  }

  SECTION("Construct from JsonArray") {
    JsonDocument doc1(200);
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(arr, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
    REQUIRE(spyingAllocator.log() == AllocatorLog() << AllocatorLog::Allocate(
                                         addPadding(doc1.memoryUsage())));
  }

  SECTION("Construct from JsonVariant") {
    JsonDocument doc1(200);
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariant>(), &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "hello");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(
                                  sizeofString(5))  // TODO: remove
                           << AllocatorLog::Allocate(sizeofString(5)));
  }
}
