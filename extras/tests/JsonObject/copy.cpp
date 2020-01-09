// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::set()") {
  DynamicJsonDocument doc1(4096);
  DynamicJsonDocument doc2(4096);

  JsonObject obj1 = doc1.to<JsonObject>();
  JsonObject obj2 = doc2.to<JsonObject>();

  SECTION("doesn't copy static string in key or value") {
    obj1["hello"] = "world";

    obj2.set(obj1);

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy local string value") {
    obj1["hello"] = std::string("world");

    obj2.set(obj1);

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy local key") {
    obj1[std::string("hello")] = "world";

    obj2.set(obj1);

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy string from deserializeJson()") {
    deserializeJson(doc1, "{'hello':'world'}");

    obj2.set(obj1);

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy string from deserializeMsgPack()") {
    deserializeMsgPack(doc1, "\x81\xA5hello\xA5world");

    obj2.set(obj1);

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("should work with JsonObjectConst") {
    obj1["hello"] = "world";

    obj2.set(static_cast<JsonObjectConst>(obj1));

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }
}
