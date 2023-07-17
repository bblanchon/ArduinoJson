// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::addPadding;
using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonDocument constructor") {
  SpyingAllocator spyingAllocator;

  SECTION("JsonDocument(size_t)") {
    { JsonDocument doc(&spyingAllocator); }
    REQUIRE(spyingAllocator.log() == AllocatorLog());
  }

  SECTION("JsonDocument(const JsonDocument&)") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      JsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
    }
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Deallocate(sizeofString(31))
                           << AllocatorLog::Deallocate(sizeofString(31)));
  }

  SECTION("JsonDocument(JsonDocument&&)") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1.set(std::string("The size of this string is 32!!"));

      JsonDocument doc2(std::move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc1.as<std::string>() == "null");
    }
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(31))
                           << AllocatorLog::Deallocate(sizeofString(31)));
  }

  SECTION("JsonDocument(JsonObject)") {
    JsonDocument doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(obj, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofPoolList())
                           << AllocatorLog::Allocate(sizeofPool()));
  }

  SECTION("Construct from JsonArray") {
    JsonDocument doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(arr, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofPoolList())
                           << AllocatorLog::Allocate(sizeofPool()));
  }

  SECTION("Construct from JsonVariant") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariant>(), &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "hello");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString(5)));
  }
}
