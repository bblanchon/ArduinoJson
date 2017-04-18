// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

static void eraseString(std::string &str) {
  char *p = const_cast<char *>(str.c_str());
  while (*p) *p++ = '*';
}

TEST_CASE("std::string") {
  DynamicJsonBuffer jb;

  SECTION("JsonBuffer_ParseArray") {
    std::string json("[\"hello\"]");
    JsonArray &array = jb.parseArray(json);
    eraseString(json);
    REQUIRE(true == array.success());
    REQUIRE(std::string("hello") == array[0]);
  }

  SECTION("JsonBuffer_ParseObject") {
    std::string json("{\"hello\":\"world\"}");
    JsonObject &object = jb.parseObject(json);
    eraseString(json);
    REQUIRE(true == object.success());
    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObject_Subscript") {
    char json[] = "{\"key\":\"value\"}";
    JsonObject &object = jb.parseObject(json);
    REQUIRE(std::string("value") == object[std::string("key")]);
  }

  SECTION("JsonObject_ConstSubscript") {
    char json[] = "{\"key\":\"value\"}";
    const JsonObject &object = jb.parseObject(json);
    REQUIRE(std::string("value") == object[std::string("key")]);
  }

  SECTION("JsonObject_SetKey") {
    JsonObject &object = jb.createObject();
    std::string key("hello");
    object.set(key, "world");
    eraseString(key);
    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObject_SetValue") {
    JsonObject &object = jb.createObject();
    std::string value("world");
    object.set("hello", value);
    eraseString(value);
    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObject_SetKeyValue") {
    JsonObject &object = jb.createObject();
    std::string key("hello");
    std::string value("world");
    object.set(key, value);
    eraseString(key);
    eraseString(value);
    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObject_SetToArraySubscript") {
    JsonArray &arr = jb.createArray();
    arr.add("world");

    JsonObject &object = jb.createObject();
    object.set(std::string("hello"), arr[0]);

    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObject_SetToObjectSubscript") {
    JsonObject &arr = jb.createObject();
    arr.set("x", "world");

    JsonObject &object = jb.createObject();
    object.set(std::string("hello"), arr["x"]);

    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObject_Get") {
    char json[] = "{\"key\":\"value\"}";
    const JsonObject &object = jb.parseObject(json);
    REQUIRE(std::string("value") ==
            object.get<const char *>(std::string("key")));
  }

  SECTION("JsonObject_GetT") {
    char json[] = "{\"key\":\"value\"}";
    const JsonObject &object = jb.parseObject(json);
    REQUIRE(std::string("value") ==
            object.get<const char *>(std::string("key")));
  }

  SECTION("JsonObject_IsT") {
    char json[] = "{\"key\":\"value\"}";
    const JsonObject &object = jb.parseObject(json);
    REQUIRE(true == object.is<const char *>(std::string("key")));
  }

  SECTION("JsonObject_CreateNestedObject") {
    std::string key = "key";
    char json[64];
    JsonObject &object = jb.createObject();
    object.createNestedObject(key);
    eraseString(key);
    object.printTo(json, sizeof(json));
    REQUIRE(std::string("{\"key\":{}}") == json);
  }

  SECTION("JsonObject_CreateNestedArray") {
    std::string key = "key";
    char json[64];
    JsonObject &object = jb.createObject();
    object.createNestedArray(key);
    eraseString(key);
    object.printTo(json, sizeof(json));
    REQUIRE(std::string("{\"key\":[]}") == json);
  }

  SECTION("JsonObject_ContainsKey") {
    char json[] = "{\"key\":\"value\"}";
    const JsonObject &object = jb.parseObject(json);
    REQUIRE(true == object.containsKey(std::string("key")));
  }

  SECTION("JsonObject_Remove") {
    char json[] = "{\"key\":\"value\"}";
    JsonObject &object = jb.parseObject(json);
    REQUIRE(1 == object.size());
    object.remove(std::string("key"));
    REQUIRE(0 == object.size());
  }

  SECTION("JsonObjectSubscript_SetKey") {
    JsonObject &object = jb.createObject();
    std::string key("hello");
    object[key] = "world";
    eraseString(key);
    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonObjectSubscript_SetValue") {
    JsonObject &object = jb.createObject();
    std::string value("world");
    object["hello"] = value;
    eraseString(value);
    REQUIRE(std::string("world") == object["hello"]);
  }

  SECTION("JsonArray_Add") {
    JsonArray &array = jb.createArray();
    std::string value("hello");
    array.add(value);
    eraseString(value);
    REQUIRE(std::string("hello") == array[0]);
  }

  SECTION("JsonArray_Set") {
    JsonArray &array = jb.createArray();
    std::string value("world");
    array.add("hello");
    array.set(0, value);
    eraseString(value);
    REQUIRE(std::string("world") == array[0]);
  }

  SECTION("JsonArraySubscript") {
    JsonArray &array = jb.createArray();
    std::string value("world");
    array.add("hello");
    array[0] = value;
    eraseString(value);
    REQUIRE(std::string("world") == array[0]);
  }

  SECTION("JsonArray_PrintTo") {
    JsonArray &array = jb.createArray();
    array.add(4);
    array.add(2);
    std::string json;
    array.printTo(json);
    REQUIRE(std::string("[4,2]") == json);
  }

  SECTION("JsonArray_PrettyPrintTo") {
    JsonArray &array = jb.createArray();
    array.add(4);
    array.add(2);
    std::string json;
    array.prettyPrintTo(json);
    REQUIRE(std::string("[\r\n  4,\r\n  2\r\n]") == json);
  }

  SECTION("JsonObject_PrintTo") {
    JsonObject &object = jb.createObject();
    object["key"] = "value";
    std::string json;
    object.printTo(json);
    REQUIRE(std::string("{\"key\":\"value\"}") == json);
  }

  SECTION("JsonObject_PrettyPrintTo") {
    JsonObject &object = jb.createObject();
    object["key"] = "value";
    std::string json;
    object.prettyPrintTo(json);
    REQUIRE(std::string("{\r\n  \"key\": \"value\"\r\n}") == json);
  }

  SECTION("JsonBuffer_GrowWhenAddingNewKey") {
    JsonObject &object = jb.createObject();
    std::string key1("hello"), key2("world");

    object[key1] = 1;
    size_t sizeBefore = jb.size();
    object[key2] = 2;
    size_t sizeAfter = jb.size();

    REQUIRE(sizeAfter - sizeBefore >= key2.size());
  }

  SECTION("JsonBuffer_DontGrowWhenReusingKey") {
    JsonObject &object = jb.createObject();
    std::string key("hello");

    object[key] = 1;
    size_t sizeBefore = jb.size();
    object[key] = 2;
    size_t sizeAfter = jb.size();

    REQUIRE(sizeBefore == sizeAfter);
  }

  SECTION("JsonBuffer_strdup") {
    std::string original("hello");
    char *copy = jb.strdup(original);
    original[0] = 'w';
    REQUIRE(std::string("hello") == copy);
  }
}
