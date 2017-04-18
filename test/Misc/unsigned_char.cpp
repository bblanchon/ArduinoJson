// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#endif

TEST_CASE("unsigned char string") {
  SECTION("JsonBuffer::parseArray") {
    unsigned char json[] = "[42]";

    StaticJsonBuffer<JSON_ARRAY_SIZE(1)> jsonBuffer;
    JsonArray& arr = jsonBuffer.parseArray(json);

    REQUIRE(true == arr.success());
  }

  SECTION("JsonBuffer::parseObject") {
    unsigned char json[] = "{\"a\":42}";

    StaticJsonBuffer<JSON_OBJECT_SIZE(1)> jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject(json);

    REQUIRE(true == obj.success());
  }

  SECTION("JsonVariant constructor") {
    unsigned char value[] = "42";

    JsonVariant variant(value);

    REQUIRE(42 == variant.as<int>());
  }

  SECTION("JsonVariant assignment operator") {
    unsigned char value[] = "42";

    JsonVariant variant(666);
    variant = value;

    REQUIRE(42 == variant.as<int>());
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonVariant::operator[]") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[key]);
  }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonVariant::operator[] const") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    const JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[key]);
  }
#endif

  SECTION("JsonVariant::operator==") {
    unsigned char comparand[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    const JsonVariant variant = "hello";

    REQUIRE(comparand == variant);
    REQUIRE(variant == comparand);
    REQUIRE_FALSE(comparand != variant);
    REQUIRE_FALSE(variant != comparand);
  }

  SECTION("JsonVariant::operator!=") {
    unsigned char comparand[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    const JsonVariant variant = "world";

    REQUIRE(comparand != variant);
    REQUIRE(variant != comparand);
    REQUIRE_FALSE(comparand == variant);
    REQUIRE_FALSE(variant == comparand);
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject::operator[]") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj[key] = "world";

    REQUIRE(std::string("world") == obj["hello"]);
  }
#endif

  SECTION("JsonObjectSubscript::operator=") {  // issue #416
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj["hello"] = value;

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObjectSubscript::set()") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj["hello"].set(value);

    REQUIRE(std::string("world") == obj["hello"]);
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject::operator[] const") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj[key]);
  }
#endif

  SECTION("JsonObject::get()") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj.get<char*>(key));
  }

  SECTION("JsonObject::set() key") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set(key, "world");

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject::set() value") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set("hello", value);

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject::set() key with decimals") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set(key, 3.14, 2);

    REQUIRE(3.14 == obj["hello"]);
  }

  SECTION("JsonObject::set key&value") {
    unsigned char key[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set(key, key);

    REQUIRE(std::string("world") == obj["world"]);
  }

  SECTION("JsonObject::containsKey()") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(true == obj.containsKey(key));
  }

  SECTION("JsonObject::remove()") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");
    obj.remove(key);

    REQUIRE(0 == obj.size());
  }

  SECTION("JsonObject::is()") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject("{\"hello\":42}");

    REQUIRE(true == obj.is<int>(key));
  }

  SECTION("JsonObject::createNestedArray()") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.createNestedArray(key);
  }

  SECTION("JsonObject::createNestedObject()") {
    unsigned char key[] = "hello";

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.createNestedObject(key);
  }

  SECTION("JsonArray::add()") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add(value);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArray::set()") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add("hello");
    arr.set(0, value);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript::set()") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add("hello");
    arr[0].set(value);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript::operator=") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add("hello");
    arr[0] = value;

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonBuffer::strdup()") {
    unsigned char value[] = "world";

    DynamicJsonBuffer jsonBuffer;
    const char* dup = jsonBuffer.strdup(value);

    REQUIRE(static_cast<const void*>(value) != static_cast<const void*>(dup));
    REQUIRE(std::string("world") == dup);
  }
}
