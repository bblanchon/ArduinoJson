// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::set()") {
  DynamicJsonDocument doc;
  JsonObject& obj = doc.to<JsonObject>();

  SECTION("int") {
    obj.set("hello", 123);

    REQUIRE(123 == obj["hello"].as<int>());
    REQUIRE(obj["hello"].is<int>());
    REQUIRE_FALSE(obj["hello"].is<bool>());
  }

  SECTION("double") {
    obj.set("hello", 123.45);

    REQUIRE(123.45 == obj["hello"].as<double>());
    REQUIRE(obj["hello"].is<double>());
    REQUIRE_FALSE(obj["hello"].is<bool>());
  }

  SECTION("bool") {
    obj.set("hello", true);

    REQUIRE(obj["hello"].as<bool>());
    REQUIRE(obj["hello"].is<bool>());
    REQUIRE_FALSE(obj["hello"].is<long>());
  }

  SECTION("const char*") {
    obj.set("hello", "h3110");

    REQUIRE(std::string("h3110") == obj["hello"].as<const char*>());
    REQUIRE(obj["hello"].is<const char*>());
    REQUIRE_FALSE(obj["hello"].is<long>());
  }

  SECTION("nested array") {
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();

    obj.set("hello", arr);

    REQUIRE(&arr == &obj["hello"].as<JsonArray>());
    REQUIRE(obj["hello"].is<JsonArray&>());
    REQUIRE_FALSE(obj["hello"].is<JsonObject&>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2;
    JsonObject& obj2 = doc2.to<JsonObject>();

    obj.set("hello", obj2);

    REQUIRE(&obj2 == &obj["hello"].as<JsonObject>());
    REQUIRE(obj["hello"].is<JsonObject&>());
    REQUIRE_FALSE(obj["hello"].is<JsonArray&>());
  }

  SECTION("array subscript") {
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();
    arr.add(42);

    obj.set("a", arr[0]);

    REQUIRE(42 == obj["a"]);
  }

  SECTION("object subscript") {
    DynamicJsonDocument doc2;
    JsonObject& obj2 = doc2.to<JsonObject>();
    obj2.set("x", 42);

    obj.set("a", obj2["x"]);

    REQUIRE(42 == obj["a"]);
  }

  SECTION("returns true when allocation succeeds") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1) + 15> doc2;
    JsonObject& obj2 = doc2.to<JsonObject>();

    REQUIRE(true == obj2.set(std::string("hello"), std::string("world")));
  }

  SECTION("returns false when allocation fails") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1) + 10> doc2;
    JsonObject& obj2 = doc2.to<JsonObject>();

    REQUIRE(false == obj2.set(std::string("hello"), std::string("world")));
  }

  SECTION("should not duplicate const char*") {
    obj.set("hello", "world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char* value") {
    obj.set("hello", const_cast<char*>("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char* key") {
    obj.set(const_cast<char*>("hello"), "world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char* key&value") {
    obj.set(const_cast<char*>("hello"), const_cast<char*>("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 12;
    REQUIRE(expectedSize <= doc.memoryUsage());
  }

  SECTION("should duplicate std::string value") {
    obj.set("hello", std::string("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string key") {
    obj.set(std::string("hello"), "world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string key&value") {
    obj.set(std::string("hello"), std::string("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 12;
    REQUIRE(expectedSize <= doc.memoryUsage());
  }
}
