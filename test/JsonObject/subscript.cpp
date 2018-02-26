// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonObject::operator[]") {
  DynamicJsonObject _object;

  SECTION("int") {
    _object["hello"] = 123;

    REQUIRE(123 == _object["hello"].as<int>());
    REQUIRE(true == _object["hello"].is<int>());
    REQUIRE(false == _object["hello"].is<bool>());
  }

  SECTION("volatile int") {  // issue #415
    volatile int i = 123;
    _object["hello"] = i;

    REQUIRE(123 == _object["hello"].as<int>());
    REQUIRE(true == _object["hello"].is<int>());
    REQUIRE(false == _object["hello"].is<bool>());
  }

  SECTION("double") {
    _object["hello"] = 123.45;

    REQUIRE(true == _object["hello"].is<double>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(123.45 == _object["hello"].as<double>());
  }

  SECTION("bool") {
    _object["hello"] = true;

    REQUIRE(true == _object["hello"].is<bool>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(true == _object["hello"].as<bool>());
  }

  SECTION("const char*") {
    _object["hello"] = "h3110";

    REQUIRE(true == _object["hello"].is<const char*>());
    REQUIRE(false == _object["hello"].is<long>());
    REQUIRE(std::string("h3110") == _object["hello"].as<const char*>());
    REQUIRE(std::string("h3110") ==
            _object["hello"].as<char*>());  // <- short hand
  }

  SECTION("array") {
    DynamicJsonArray arr;

    _object["hello"] = arr;

    REQUIRE(&arr == &_object["hello"].as<JsonArray&>());
    REQUIRE(&arr == &_object["hello"].as<JsonArray>());  // <- short hand
    REQUIRE(&arr == &_object["hello"].as<const JsonArray&>());
    REQUIRE(&arr == &_object["hello"].as<const JsonArray>());  // <- short hand
    REQUIRE(true == _object["hello"].is<JsonArray&>());
    REQUIRE(true == _object["hello"].is<JsonArray>());
    REQUIRE(true == _object["hello"].is<const JsonArray&>());
    REQUIRE(true == _object["hello"].is<const JsonArray>());
    REQUIRE(false == _object["hello"].is<JsonObject&>());
  }

  SECTION("object") {
    DynamicJsonObject obj;

    _object["hello"] = obj;

    REQUIRE(&obj == &_object["hello"].as<JsonObject&>());
    REQUIRE(&obj == &_object["hello"].as<JsonObject>());  // <- short hand
    REQUIRE(&obj == &_object["hello"].as<const JsonObject&>());
    REQUIRE(&obj == &_object["hello"].as<const JsonObject>());  // <- short hand
    REQUIRE(true == _object["hello"].is<JsonObject&>());
    REQUIRE(true == _object["hello"].is<JsonObject>());
    REQUIRE(true == _object["hello"].is<const JsonObject&>());
    REQUIRE(true == _object["hello"].is<const JsonObject>());
    REQUIRE(false == _object["hello"].is<JsonArray&>());
  }

  SECTION("array subscript") {
    DynamicJsonArray arr;
    arr.add(42);

    _object["a"] = arr[0];

    REQUIRE(42 == _object["a"]);
  }

  SECTION("object subscript") {
    DynamicJsonObject obj;
    obj.set("x", 42);

    _object["a"] = obj["x"];

    REQUIRE(42 == _object["a"]);
  }

  SECTION("char key[]") {  // issue #423
    char key[] = "hello";
    _object[key] = 42;
    REQUIRE(42 == _object[key]);
  }

  SECTION("should not duplicate const char*") {
    _object["hello"] = "world";
    const size_t expectedSize = JSON_OBJECT_SIZE(1);
    REQUIRE(expectedSize == _object.memoryUsage());
  }

  SECTION("should duplicate char* value") {
    _object["hello"] = const_cast<char*>("world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == _object.memoryUsage());
  }

  SECTION("should duplicate char* key") {
    _object[const_cast<char*>("hello")] = "world";
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == _object.memoryUsage());
  }

  SECTION("should duplicate char* key&value") {
    _object[const_cast<char*>("hello")] = const_cast<char*>("world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 12;
    REQUIRE(expectedSize <= _object.memoryUsage());
  }

  SECTION("should duplicate std::string value") {
    _object["hello"] = std::string("world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == _object.memoryUsage());
  }

  SECTION("should duplicate std::string key") {
    _object[std::string("hello")] = "world";
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == _object.memoryUsage());
  }

  SECTION("should duplicate std::string key&value") {
    _object[std::string("hello")] = std::string("world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 12;
    REQUIRE(expectedSize <= _object.memoryUsage());
  }
}
