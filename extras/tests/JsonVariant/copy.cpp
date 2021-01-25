// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::set(JsonVariant)") {
  DynamicJsonDocument doc1(4096);
  DynamicJsonDocument doc2(4096);
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
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 0);
    REQUIRE(doc2.memoryUsage() == 0);
  }

  SECTION("stores char* by copy") {
    char str[] = "hello!!";

    var1.set(str);
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == JSON_STRING_SIZE(7));
    REQUIRE(doc2.memoryUsage() == JSON_STRING_SIZE(7));
  }

  SECTION("stores std::string by copy") {
    var1.set(std::string("hello!!"));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == JSON_STRING_SIZE(7));
    REQUIRE(doc2.memoryUsage() == JSON_STRING_SIZE(7));
  }

  SECTION("stores Serialized<const char*> by reference") {
    var1.set(serialized("hello!!", 8));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == 0);
    REQUIRE(doc2.memoryUsage() == 0);
  }

  SECTION("stores Serialized<char*> by copy") {
    char str[] = "hello!!";
    var1.set(serialized(str, 7));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == JSON_STRING_SIZE(7));
    REQUIRE(doc2.memoryUsage() == JSON_STRING_SIZE(7));
  }

  SECTION("stores Serialized<std::string> by copy") {
    var1.set(serialized(std::string("hello!!")));
    var2.set(var1);

    REQUIRE(doc1.memoryUsage() == JSON_STRING_SIZE(7));
    REQUIRE(doc2.memoryUsage() == JSON_STRING_SIZE(7));
  }
}
