// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

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

    StaticJsonArray<JSON_ARRAY_SIZE(1)> arr;
    bool success = deserializeJson(arr, vla);

    REQUIRE(true == success);
  }

  SECTION("ParseObject") {
    int i = 16;
    char vla[i];
    strcpy(vla, "{\"a\":42}");

    StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
    deserializeJson(obj, vla);

    REQUIRE(true == obj.success());
  }

  SECTION("Parse") {
    int i = 16;
    char vla[i];
    strcpy(vla, "42");

    StaticJsonVariant<> variant;
    deserializeJson(variant, vla);

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

    DynamicJsonVariant variant;
    deserializeJson(variant, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[vla]);
  }
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonVariant_Subscript_Const") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonVariant variant;
    deserializeJson(variant, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == variant[vla]);
  }
#endif

  SECTION("JsonVariant_Equals") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonVariant variant;
    variant = "hello";

    REQUIRE((vla == variant));
    REQUIRE((variant == vla));
    REQUIRE_FALSE((vla != variant));
    REQUIRE_FALSE((variant != vla));
  }

  SECTION("JsonVariant_Differs") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonVariant variant;
    variant = "world";

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

    DynamicJsonObject obj;
    obj[vla] = "world";

    REQUIRE(std::string("world") == obj["hello"]);
  }
#endif

  SECTION("JsonObject_Subscript_Assign") {  // issue #416
    int i = 32;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonObject obj;
    obj["hello"] = vla;

    REQUIRE(std::string("world") == obj["hello"].as<char*>());
  }

  SECTION("JsonObject_Subscript_Set") {
    int i = 32;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonObject obj;
    obj["hello"].set(vla);

    REQUIRE(std::string("world") == obj["hello"].as<char*>());
  }

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
  SECTION("JsonObject_Subscript_Const") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj[vla]);
  }
#endif

  SECTION("JsonObject_Get") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");

    REQUIRE(std::string("world") == obj.get<char*>(vla));
  }

  SECTION("JsonObject_Set_Key") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    obj.set(vla, "world");

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject_Set_Value") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonObject obj;
    obj.set("hello", vla);

    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("JsonObject_Set_KeyAndValue") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonObject obj;
    obj.set(vla, vla);

    REQUIRE(std::string("world") == obj["world"]);
  }

  SECTION("JsonObject_ContainsKey") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");

    REQUIRE(true == obj.containsKey(vla));
  }

  SECTION("JsonObject_Remove") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":\"world\"}");
    obj.remove(vla);

    REQUIRE(0 == obj.size());
  }

  SECTION("JsonObject_Is") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    deserializeJson(obj, "{\"hello\":42}");

    REQUIRE(true == obj.is<int>(vla));
  }

  SECTION("JsonObject_CreateNestedArray") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    obj.createNestedArray(vla);
  }

  SECTION("JsonObject_CreateNestedObject") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonObject obj;
    obj.createNestedObject(vla);
  }

  SECTION("JsonArray_Add") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonArray arr;
    arr.add(vla);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArray_Set") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonArray arr;
    arr.add("hello");
    arr.set(0, vla);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript_Set") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonArray arr;
    arr.add("hello");
    arr[0].set(vla);

    REQUIRE(std::string("world") == arr[0]);
  }

  SECTION("JsonArraySubscript_Assign") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    DynamicJsonArray arr;
    arr.add("hello");
    arr[0] = vla;

    REQUIRE(std::string("world") == arr[0]);
  }
}
#endif
