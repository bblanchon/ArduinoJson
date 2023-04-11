// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::sizeofObject;

TEST_CASE("JsonObject::set()") {
  JsonDocument doc1(4096);
  JsonDocument doc2(4096);

  JsonObject obj1 = doc1.to<JsonObject>();
  JsonObject obj2 = doc2.to<JsonObject>();

  SECTION("doesn't copy static string in key or value") {
    obj1["hello"] = "world";

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy local string value") {
    obj1["hello"] = std::string("world");

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy local key") {
    obj1[std::string("hello")] = "world";

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy string from deserializeJson()") {
    deserializeJson(doc1, "{'hello':'world'}");

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("copy string from deserializeMsgPack()") {
    deserializeMsgPack(doc1, "\x81\xA5hello\xA5world");

    bool success = obj2.set(obj1);

    REQUIRE(success == true);
    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("should work with JsonObjectConst") {
    obj1["hello"] = "world";

    obj2.set(static_cast<JsonObjectConst>(obj1));

    REQUIRE(doc1.memoryUsage() == doc2.memoryUsage());
    REQUIRE(obj2["hello"] == std::string("world"));
  }

  SECTION("destination too small to store the key") {
    JsonDocument doc3(sizeofObject(1));
    JsonObject obj3 = doc3.to<JsonObject>();

    obj1["a"] = 1;
    obj1["b"] = 2;

    bool success = obj3.set(obj1);

    REQUIRE(success == false);
    REQUIRE(doc3.as<std::string>() == "{\"a\":1}");
  }

  SECTION("destination too small to store the value") {
    JsonDocument doc3(sizeofObject(1));
    JsonObject obj3 = doc3.to<JsonObject>();

    obj1["hello"][1] = "world";

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
