// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include "Allocators.hpp"

using ArduinoJson::detail::sizeofString;

TEST_CASE("JsonVariant::set(JsonVariant)") {
  ControllableAllocator allocator;
  SpyingAllocator spyingAllocator(&allocator);
  JsonDocument doc1(4096, &spyingAllocator);
  JsonDocument doc2(4096, &spyingAllocator);
  JsonVariant var1 = doc1.to<JsonVariant>();
  JsonVariant var2 = doc2.to<JsonVariant>();

  SECTION("stores JsonArray by copy") {
    JsonArray arr = doc2.to<JsonArray>();
    JsonObject obj = arr.createNestedObject();
    obj["hello"] = "world";

    var1.set(arr);

    arr[0] = 666;
    REQUIRE(var1.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("stores JsonObject by copy") {
    JsonObject obj = doc2.to<JsonObject>();
    JsonArray arr = obj.createNestedArray("value");
    arr.add(42);

    var1.set(obj);

    obj["value"] = 666;
    REQUIRE(var1.as<std::string>() == "{\"value\":[42]}");
  }

  SECTION("stores const char* by reference") {
    var1.set("hello!!");
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 0);
    REQUIRE(doc2.memoryUsage() == 0);
    REQUIRE(spyingAllocator.log() == AllocatorLog());
  }

  SECTION("stores char* by copy") {
    char str[] = "hello!!";
    var1.set(str);
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == sizeofString(7));
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString((7))));
  }

  SECTION("fails gracefully if string allocation fails") {
    char str[] = "hello!!";
    var1.set(str);
    allocator.disable();
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == 0);
    REQUIRE(doc2.overflowed() == true);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::AllocateFail(sizeofString((7))));
  }

  SECTION("stores std::string by copy") {
    var1.set(std::string("hello!!"));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == sizeofString(7));
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString((7))));
  }

  SECTION("stores Serialized<const char*> by copy") {
    var1.set(serialized("hello!!", 7));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == sizeofString(7));
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString((7))));
  }

  SECTION("stores Serialized<char*> by copy") {
    char str[] = "hello!!";
    var1.set(serialized(str, 7));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == sizeofString(7));
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString((7))));
  }

  SECTION("stores Serialized<std::string> by copy") {
    var1.set(serialized(std::string("hello!!")));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == sizeofString(7));
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::Allocate(sizeofString((7))));
  }

  SECTION("fails gracefully if raw string allocation fails") {
    var1.set(serialized(std::string("hello!!")));
    allocator.disable();
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == sizeofString(7));
    REQUIRE(doc2.memoryUsage() == 0);
    REQUIRE(doc2.overflowed() == true);
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog() << AllocatorLog::AllocateFail(sizeofString((7))));
  }

  SECTION("destination is unbound") {
    JsonVariant unboundVariant;

    unboundVariant.set(var1);

    REQUIRE(unboundVariant.isUnbound());
    REQUIRE(unboundVariant.isNull());
  }
}
