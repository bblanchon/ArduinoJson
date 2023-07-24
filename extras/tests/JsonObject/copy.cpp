// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonObject::set()") {
  SpyingAllocator allocator;
  JsonDocument doc1(&allocator);
  JsonDocument doc2(&allocator);

  JsonObject obj1 = doc1.to<JsonObject>();
  JsonObject obj2 = doc2.to<JsonObject>();

  SECTION("doesn't copy static string in key or value") {
    obj1["hello"] = "world";
    allocator.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == std::string("world"));
    REQUIRE(allocator.log() == AllocatorLog()
                                   << AllocatorLog::Allocate(sizeofPool()));
  }

  SECTION("copy local string value") {
    obj1["hello"] = std::string("world");
    allocator.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == std::string("world"));
    REQUIRE(allocator.log() == AllocatorLog()
                                   << AllocatorLog::Allocate(sizeofPool())
                                   << AllocatorLog::Allocate(sizeofString(5)));
  }

  SECTION("copy local key") {
    obj1[std::string("hello")] = "world";
    allocator.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == std::string("world"));
    REQUIRE(allocator.log() == AllocatorLog()
                                   << AllocatorLog::Allocate(sizeofString(5))
                                   << AllocatorLog::Allocate(sizeofPool()));
  }

  SECTION("copy string from deserializeJson()") {
    deserializeJson(doc1, "{'hello':'world'}");
    allocator.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == std::string("world"));
    REQUIRE(allocator.log() == AllocatorLog()
                                   << AllocatorLog::Allocate(sizeofString(5))
                                   << AllocatorLog::Allocate(sizeofPool())
                                   << AllocatorLog::Allocate(sizeofString(5)));
  }

  SECTION("copy string from deserializeMsgPack()") {
    deserializeMsgPack(doc1, "\x81\xA5hello\xA5world");
    allocator.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == std::string("world"));
    REQUIRE(allocator.log() == AllocatorLog()
                                   << AllocatorLog::Allocate(sizeofString(5))
                                   << AllocatorLog::Allocate(sizeofPool())
                                   << AllocatorLog::Allocate(sizeofString(5)));
  }

  SECTION("should work with JsonObjectConst") {
    obj1["hello"] = "world";

    obj2.set(static_cast<JsonObjectConst>(obj1));

    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy fails in the middle of an object") {
    TimebombAllocator timebomb(2);
    JsonDocument doc3(&timebomb);
    JsonObject obj3 = doc3.to<JsonObject>();

    obj1[std::string("a")] = 1;
    obj1[std::string("b")] = 2;

    bool success = obj3.set(obj1);

    REQUIRE(success == false);
    REQUIRE(doc3.as<std::string>() == "{\"a\":1}");
  }

  SECTION("copy fails in the middle of an array") {
    TimebombAllocator timebomb(1);
    JsonDocument doc3(&timebomb);
    JsonObject obj3 = doc3.to<JsonObject>();

    obj1["hello"][0] = std::string("world");

    bool success = obj3.set(obj1);

    REQUIRE(success == false);
    REQUIRE(doc3.as<std::string>() == "{\"hello\":[null]}");
  }

  SECTION("destination is null") {
    JsonObject null;
    obj1["hello"] = "world";

    bool success = null.set(obj1);

    REQUIRE(success == false);
  }

  SECTION("source is null") {
    JsonObject null;
    obj1["hello"] = "world";

    bool success = obj1.set(null);

    REQUIRE(success == false);
  }
}
