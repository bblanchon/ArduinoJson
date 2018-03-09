// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#endif

TEST_CASE("unsigned char string") {
  SECTION("JsonBuffer::parseArray") {
    unsigned char json[] = "[42]";

    StaticJsonArray<JSON_ARRAY_SIZE(1)> arr;
    JsonError err = deserializeJson(arr, json);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("JsonBuffer::parseObject") {
    unsigned char json[] = "{\"a\":42}";

    StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
    JsonError err = deserializeJson(obj, json);

    REQUIRE(err == JsonError::Ok);
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

    DynamicJsonVariant variant;
    deserializeJson(variant, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[key]);
  }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonVariant::operator[] const") {
    unsigned char key[] = "hello";

    DynamicJsonVariant variant;
    deserializeJson(variant, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[key]);
  }
#endif

  SECTION("JsonVariant::operator==") {
    unsigned char comparand[] = "hello";

    DynamicJsonVariant variant;
    variant = "hello";

    REQUIRE(comparand == variant);
    REQUIRE(variant == comparand);
    REQUIRE_FALSE(comparand != variant);
    REQUIRE_FALSE(variant != comparand);
  }

  SECTION("JsonVariant::operator!=") {
    unsigned char comparand[] = "hello";

    DynamicJsonVariant variant;
    variant = "world";

    REQUIRE(comparand != variant);
    REQUIRE(variant != comparand);
    REQUIRE_FALSE(comparand == variant);
    REQUIRE_FALSE(variant == comparand);
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject::operator[]") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    obj[key] = "world";

    REQUIRE(std::string("world") == obj["hello"]);
  }
#endif

  SECTION("JsonObjectSubscript::operator=") {  // issue #416
    unsigned char value[] = "world";

    DynamicJsonObject obj;
    obj["hello"] = value;

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObjectSubscript::set()") {
    unsigned char value[] = "world";

    DynamicJsonObject obj;
    obj["hello"].set(value);

    REQUIRE(std::string("world") == obj["hello"]);
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject::operator[] const") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj[key]);
  }
#endif

  SECTION("JsonObject::get()") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj.get<char*>(key));
  }

  SECTION("JsonObject::set() key") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    obj.set(key, "world");

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject::set() value") {
    unsigned char value[] = "world";

    DynamicJsonObject obj;
    obj.set("hello", value);

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject::set key&value") {
    unsigned char key[] = "world";

    DynamicJsonObject obj;
    obj.set(key, key);

    REQUIRE(std::string("world") == obj["world"]);
  }

  SECTION("JsonObject::containsKey()") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");

    REQUIRE(true == obj.containsKey(key));
  }

  SECTION("JsonObject::remove()") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");
    obj.remove(key);

    REQUIRE(0 == obj.size());
  }

  SECTION("JsonObject::is()") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":42}");

    REQUIRE(true == obj.is<int>(key));
  }

  SECTION("JsonObject::createNestedArray()") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    obj.createNestedArray(key);
  }

  SECTION("JsonObject::createNestedObject()") {
    unsigned char key[] = "hello";

    DynamicJsonObject obj;
    obj.createNestedObject(key);
  }

  SECTION("JsonArray::add()") {
    unsigned char value[] = "world";

    DynamicJsonArray arr;
    arr.add(value);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArray::set()") {
    unsigned char value[] = "world";

    DynamicJsonArray arr;
    arr.add("hello");
    arr.set(0, value);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript::set()") {
    unsigned char value[] = "world";

    DynamicJsonArray arr;
    arr.add("hello");
    arr[0].set(value);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript::operator=") {
    unsigned char value[] = "world";

    DynamicJsonArray arr;
    arr.add("hello");
    arr[0] = value;

    REQUIRE(std::string("world") == arr[0]);
  }
}
