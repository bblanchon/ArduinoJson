// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wvla-extension"
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvla"
#else
#define VLA_NOT_SUPPORTED
#endif

#ifndef VLA_NOT_SUPPORTED

TEST_CASE("Variable Length Array") {
  SECTION("ParseArray") {
    int i = 8;
    char vla[i];
    strcpy(vla, "[42]");

    StaticJsonBuffer<JSON_ARRAY_SIZE(1)> jsonBuffer;
    JsonArray& arr = jsonBuffer.parseArray(vla);

    REQUIRE(true == arr.success());
  }

  SECTION("ParseObject") {
    int i = 16;
    char vla[i];
    strcpy(vla, "{\"a\":42}");

    StaticJsonBuffer<JSON_OBJECT_SIZE(1)> jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject(vla);

    REQUIRE(true == obj.success());
  }

  SECTION("Parse") {
    int i = 16;
    char vla[i];
    strcpy(vla, "42");

    StaticJsonBuffer<1> jsonBuffer;
    JsonVariant variant = jsonBuffer.parse(vla);

    REQUIRE(42 == variant.as<int>());
  }

  SECTION("JsonVariant_Constructor") {
    int i = 16;
    char vla[i];
    strcpy(vla, "42");

    JsonVariant variant(vla);

    REQUIRE(42 == variant.as<int>());
  }

  SECTION("JsonVariant_Assign") {
    int i = 16;
    char vla[i];
    strcpy(vla, "42");

    JsonVariant variant(666);
    variant = vla;

    REQUIRE(42 == variant.as<int>());
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonVariant_Subscript") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[vla]);
  }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonVariant_Subscript_Const") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    const JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[vla]);
  }
#endif

  SECTION("JsonVariant_Equals") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    const JsonVariant variant = "hello";

    REQUIRE((vla == variant));
    REQUIRE((variant == vla));
    REQUIRE_FALSE((vla != variant));
    REQUIRE_FALSE((variant != vla));
  }

  SECTION("JsonVariant_Differs") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    const JsonVariant variant = "world";

    REQUIRE((vla != variant));
    REQUIRE((variant != vla));
    REQUIRE_FALSE((vla == variant));
    REQUIRE_FALSE((variant == vla));
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject_Subscript") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj[vla] = "world";

    REQUIRE(std::string("world") == obj["hello"]);
  }
#endif

  SECTION("JsonObject_Subscript_Assign") {  // issue #416
    int i = 32;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj["hello"] = vla;

    REQUIRE(std::string("world") == obj["hello"].as<char*>());
  }

  SECTION("JsonObject_Subscript_Set") {
    int i = 32;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj["hello"].set(vla);

    REQUIRE(std::string("world") == obj["hello"].as<char*>());
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject_Subscript_Const") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj[vla]);
  }
#endif

  SECTION("JsonObject_Get") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj.get<char*>(vla));
  }

  SECTION("JsonObject_Set_Key") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set(vla, "world");

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject_Set_Value") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set("hello", vla);

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject_Set_KeyAndValue") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.set(vla, vla);

    REQUIRE(std::string("world") == obj["world"]);
  }

  SECTION("JsonObject_ContainsKey") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

    REQUIRE(true == obj.containsKey(vla));
  }

  SECTION("JsonObject_Remove") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");
    obj.remove(vla);

    REQUIRE(0 == obj.size());
  }

  SECTION("JsonObject_Is") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.parseObject("{\"hello\":42}");

    REQUIRE(true == obj.is<int>(vla));
  }

  SECTION("JsonObject_CreateNestedArray") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.createNestedArray(vla);
  }

  SECTION("JsonObject_CreateNestedObject") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonBuffer jsonBuffer;
    JsonObject& obj = jsonBuffer.createObject();
    obj.createNestedObject(vla);
  }

  SECTION("JsonArray_Add") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add(vla);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArray_Set") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add("hello");
    arr.set(0, vla);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript_Set") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add("hello");
    arr[0].set(vla);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript_Assign") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.add("hello");
    arr[0] = vla;

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonBuffer_strdup") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonBuffer jsonBuffer;
    const char* dup = jsonBuffer.strdup(vla);

    REQUIRE(static_cast<const void*>(vla) != static_cast<const void*>(dup));
    REQUIRE(std::string("world") == dup);
  }
}
#endif
