// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonArray::operator[]") {
  DynamicJsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  array.add(0);

  SECTION("int") {
    array[0] = 123;
    REQUIRE(123 == array[0].as<int>());
    REQUIRE(true == array[0].is<int>());
    REQUIRE(false == array[0].is<bool>());
  }

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("long long") {
    array[0] = 9223372036854775807;
    REQUIRE(9223372036854775807 == array[0].as<long long>());
    REQUIRE(true == array[0].is<int>());
    REQUIRE(false == array[0].is<bool>());
  }
#endif

  SECTION("double") {
    array[0] = 123.45;
    REQUIRE(123.45 == array[0].as<double>());
    REQUIRE(true == array[0].is<double>());
    REQUIRE(false == array[0].is<int>());
  }

  SECTION("bool") {
    array[0] = true;
    REQUIRE(true == array[0].as<bool>());
    REQUIRE(true == array[0].is<bool>());
    REQUIRE(false == array[0].is<int>());
  }

  SECTION("const char*") {
    const char* str = "hello";

    array[0] = str;
    REQUIRE(str == array[0].as<const char*>());
    REQUIRE(str == array[0].as<char*>());  // <- short hand
    REQUIRE(true == array[0].is<const char*>());
    REQUIRE(false == array[0].is<int>());
  }

  SECTION("nested array") {
    DynamicJsonDocument doc2;
    JsonArray arr2 = doc2.to<JsonArray>();

    array[0] = arr2;

    REQUIRE(arr2 == array[0].as<JsonArray>());
    REQUIRE(arr2 == array[0].as<JsonArray>());  // <- short hand
    // REQUIRE(arr2 == array[0].as<const JsonArray>());
    // REQUIRE(arr2 == array[0].as<const JsonArray>());  // <- short hand
    REQUIRE(true == array[0].is<JsonArray>());
    REQUIRE(false == array[0].is<int>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    array[0] = obj;

    REQUIRE(obj == array[0].as<JsonObject>());
    REQUIRE(obj == array[0].as<const JsonObject>());  // <- short hand
    REQUIRE(true == array[0].is<JsonObject>());
    REQUIRE(false == array[0].is<int>());
  }

  SECTION("array subscript") {
    DynamicJsonDocument doc2;
    JsonArray arr2 = doc2.to<JsonArray>();
    const char* str = "hello";

    arr2.add(str);

    array[0] = arr2[0];

    REQUIRE(str == array[0]);
  }

  SECTION("object subscript") {
    const char* str = "hello";
    DynamicJsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    obj["x"] = str;

    array[0] = obj["x"];

    REQUIRE(str == array[0]);
  }

  SECTION("should not duplicate const char*") {
    array[0] = "world";
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char*") {
    array[0] = const_cast<char*>("world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string") {
    array[0] = std::string("world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("array[0].to<JsonObject>()") {
    JsonObject obj = array[0].to<JsonObject>();
    REQUIRE(obj.isNull() == false);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("set(VLA)") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    array.add("hello");
    array[0].set(vla);

    REQUIRE(std::string("world") == array[0]);
  }

  SECTION("operator=(VLA)") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    array.add("hello");
    array[0] = vla;

    REQUIRE(std::string("world") == array[0]);
  }
#endif
}
