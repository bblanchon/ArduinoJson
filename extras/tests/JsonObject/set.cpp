// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonObject::set()") {
  SpyingAllocator spy;
  JsonDocument doc1(&spy);
  JsonDocument doc2(&spy);

  JsonObject obj1 = doc1.to<JsonObject>();
  JsonObject obj2 = doc2.to<JsonObject>();

  SECTION("doesn't copy static string in key or value") {
    obj1["hello"] = "world";
    spy.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == "world"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }

  SECTION("copy local string value") {
    obj1["hello"] = "world"_s;
    spy.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == "world"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("copy local key") {
    obj1["hello"_s] = "world";
    spy.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == "world"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofPool()),
                         });
  }

  SECTION("copy string from deserializeJson()") {
    deserializeJson(doc1, "{'hello':'world'}");
    spy.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == "world"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("copy string from deserializeMsgPack()") {
    deserializeMsgPack(doc1, "\x81\xA5hello\xA5world");
    spy.clearLog();

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(obj2["hello"] == "world"_s);
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofString("hello")),
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SECTION("should work with JsonObjectConst") {
    obj1["hello"] = "world";

    obj2.set(static_cast<JsonObjectConst>(obj1));

    REQUIRE(obj2["hello"] == "world"_s);
  }

  SECTION("copy fails in the middle of an object") {
    TimebombAllocator timebomb(2);
    JsonDocument doc3(&timebomb);
    JsonObject obj3 = doc3.to<JsonObject>();

    obj1["a"_s] = 1;
    obj1["b"_s] = 2;

    bool success = obj3.set(obj1);

    REQUIRE(success == false);
    REQUIRE(doc3.as<std::string>() == "{\"a\":1}");
  }

  SECTION("copy fails in the middle of an array") {
    TimebombAllocator timebomb(1);
    JsonDocument doc3(&timebomb);
    JsonObject obj3 = doc3.to<JsonObject>();

    obj1["hello"][0] = "world"_s;

    bool success = obj3.set(obj1);

    REQUIRE(success == false);
    REQUIRE(doc3.as<std::string>() == "{\"hello\":[]}");
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
