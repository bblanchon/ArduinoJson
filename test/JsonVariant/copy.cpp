// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::set(JsonVariant)") {
  DynamicJsonDocument doc1;
  DynamicJsonDocument doc2;
  JsonVariant var1 = doc1.to<JsonVariant>();
  JsonVariant var2 = doc2.to<JsonVariant>();

  SECTION("stores JsonArray by copy") {
    JsonArray arr = doc2.to<JsonArray>();

    arr.add(42);
    var1.set(doc2.as<JsonVariant>());
    arr[0] = 666;

    REQUIRE(var1.as<std::string>() == "[42]");
  }

  SECTION("stores JsonObject by copy") {
    JsonObject obj = doc2.to<JsonObject>();

    obj["value"] = 42;
    var1.set(doc2.as<JsonVariant>());
    obj["value"] = 666;

    REQUIRE(var1.as<std::string>() == "{\"value\":42}");
  }

  SECTION("stores const char* by reference") {
    var1.set("hello!!");
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 0);
    REQUIRE(doc2.memoryUsage() == 0);
  }

  SECTION("stores char* by copy") {
    char str[] = "hello!!";

    var1.set(str);
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 8);
    REQUIRE(doc2.memoryUsage() == 8);
  }

  SECTION("stores std::string by copy") {
    var1.set(std::string("hello!!"));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 8);
    REQUIRE(doc2.memoryUsage() == 8);
  }

  SECTION("stores Serialized<const char*> by reference") {
    var1.set(serialized("hello!!", 8));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 0);
    REQUIRE(doc2.memoryUsage() == 0);
  }

  SECTION("stores Serialized<char*> by copy") {
    char str[] = "hello!!";
    var1.set(serialized(str, 8));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 8);
    REQUIRE(doc2.memoryUsage() == 8);
  }

  SECTION("stores Serialized<std::string> by copy") {
    var1.set(serialized(std::string("hello!!!")));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 8);
    REQUIRE(doc2.memoryUsage() == 8);
  }
}
