// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

TEST_CASE("JsonObject::set()") {
  DynamicJsonBuffer jb;
  JsonObject& _object = jb.createObject();

  SECTION("int") {
    _object.set("hello", 123);

    REQUIRE(123 == _object["hello"].as<int>());
    REQUIRE(_object["hello"].is<int>());
    REQUIRE_FALSE(_object["hello"].is<bool>());
  }

  SECTION("double") {
    _object.set("hello", 123.45);

    REQUIRE(123.45 == _object["hello"].as<double>());
    REQUIRE(_object["hello"].is<double>());
    REQUIRE_FALSE(_object["hello"].is<bool>());
  }

  SECTION("bool") {
    _object.set("hello", true);

    REQUIRE(_object["hello"].as<bool>());
    REQUIRE(_object["hello"].is<bool>());
    REQUIRE_FALSE(_object["hello"].is<long>());
  }

  SECTION("const char*") {
    _object.set("hello", "h3110");

    REQUIRE(std::string("h3110") == _object["hello"].as<const char*>());
    REQUIRE(_object["hello"].is<const char*>());
    REQUIRE_FALSE(_object["hello"].is<long>());
  }

  SECTION("nested array") {
    JsonArray& arr = jb.createArray();

    _object.set("hello", arr);

    REQUIRE(&arr == &_object["hello"].as<JsonArray>());
    REQUIRE(_object["hello"].is<JsonArray&>());
    REQUIRE_FALSE(_object["hello"].is<JsonObject&>());
  }

  SECTION("nested object") {
    JsonObject& obj = jb.createObject();

    _object.set("hello", obj);

    REQUIRE(&obj == &_object["hello"].as<JsonObject>());
    REQUIRE(_object["hello"].is<JsonObject&>());
    REQUIRE_FALSE(_object["hello"].is<JsonArray&>());
  }

  SECTION("array subscript") {
    JsonArray& arr = jb.createArray();
    arr.add(42);

    _object.set("a", arr[0]);

    REQUIRE(42 == _object["a"]);
  }

  SECTION("object subscript") {
    JsonObject& obj = jb.createObject();
    obj.set("x", 42);

    _object.set("a", obj["x"]);

    REQUIRE(42 == _object["a"]);
  }

  SECTION("returns true when allocation succeeds") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 15> jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();

    REQUIRE(true == obj.set(std::string("hello"), std::string("world")));
  }

  SECTION("returns false when allocation fails") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 10> jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();

    REQUIRE(false == obj.set(std::string("hello"), std::string("world")));
  }

  SECTION("should not duplicate const char*") {
    _object.set("hello", "world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1);
    REQUIRE(expectedSize == jb.size());
  }

  SECTION("should duplicate char* value") {
    _object.set("hello", const_cast<char*>("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == jb.size());
  }

  SECTION("should duplicate char* key") {
    _object.set(const_cast<char*>("hello"), "world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == jb.size());
  }

  SECTION("should duplicate char* key&value") {
    _object.set(const_cast<char*>("hello"), const_cast<char*>("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 12;
    REQUIRE(expectedSize <= jb.size());
  }

  SECTION("should duplicate std::string value") {
    _object.set("hello", std::string("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == jb.size());
  }

  SECTION("should duplicate std::string key") {
    _object.set(std::string("hello"), "world");
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 6;
    REQUIRE(expectedSize == jb.size());
  }

  SECTION("should duplicate std::string key&value") {
    _object.set(std::string("hello"), std::string("world"));
    const size_t expectedSize = JSON_OBJECT_SIZE(1) + 12;
    REQUIRE(expectedSize <= jb.size());
  }
}
