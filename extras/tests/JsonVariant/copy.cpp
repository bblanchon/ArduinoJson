// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include "Allocators.hpp"

TEST_CASE("JsonVariant::set(JsonVariant)") {
  KillswitchAllocator killswitch;
  SpyingAllocator spyingAllocator(&killswitch);
  JsonDocument doc1(&spyingAllocator);
  JsonDocument doc2(&spyingAllocator);
  JsonVariant var1 = doc1.to<JsonVariant>();
  JsonVariant var2 = doc2.to<JsonVariant>();

  SECTION("stores JsonArray by copy") {
    JsonArray arr = doc2.to<JsonArray>();
    JsonObject obj = arr.add<JsonObject>();
    obj["hello"] = "world";

    var1.set(arr);

    arr[0] = 666;
    REQUIRE(var1.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("stores JsonObject by copy") {
    JsonObject obj = doc2.to<JsonObject>();
    JsonArray arr = obj["value"].to<JsonArray>();
    arr.add(42);

    var1.set(obj);

    obj["value"] = 666;
    REQUIRE(var1.as<std::string>() == "{\"value\":[42]}");
  }

  SECTION("stores const char* by reference") {
    var1.set("hello!!");
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("stores char* by copy") {
    char str[] = "hello!!";
    var1.set(str);
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello!!")),
                                     });
  }

  SECTION("fails gracefully if string allocation fails") {
    char str[] = "hello!!";
    var1.set(str);
    killswitch.on();
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc2.overflowed() == true);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         AllocateFail(sizeofString("hello!!")),
                                     });
  }

  SECTION("stores std::string by copy") {
    var1.set(std::string("hello!!"));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello!!")),
                                     });
  }

  SECTION("stores Serialized<const char*> by copy") {
    var1.set(serialized("hello!!", 7));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello!!")),
                                     });
  }

  SECTION("stores Serialized<char*> by copy") {
    char str[] = "hello!!";
    var1.set(serialized(str, 7));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello!!")),
                                     });
  }

  SECTION("stores Serialized<std::string> by copy") {
    var1.set(serialized(std::string("hello!!")));
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello!!")),
                                     });
  }

  SECTION("fails gracefully if raw string allocation fails") {
    var1.set(serialized(std::string("hello!!")));
    killswitch.on();
    spyingAllocator.clearLog();

    var2.set(var1);

    REQUIRE(doc2.overflowed() == true);
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         AllocateFail(sizeofString("hello!!")),
                                     });
  }

  SECTION("destination is unbound") {
    JsonVariant unboundVariant;

    unboundVariant.set(var1);

    REQUIRE(unboundVariant.isUnbound());
    REQUIRE(unboundVariant.isNull());
  }
}
