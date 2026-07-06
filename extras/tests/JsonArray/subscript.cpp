// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <stdint.h>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonArray::operator[]") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonArray array = doc.to<JsonArray>();

  SUBCASE("Pad with null") {
    array[2] = 2;
    array[5] = 5;
    REQUIRE(array.size() == 6);
    REQUIRE(array[0].isNull() == true);
    REQUIRE(array[1].isNull() == true);
    REQUIRE(array[2].isNull() == false);
    REQUIRE(array[3].isNull() == true);
    REQUIRE(array[4].isNull() == true);
    REQUIRE(array[5].isNull() == false);
    REQUIRE(array[2] == 2);
    REQUIRE(array[5] == 5);
  }

  SUBCASE("int") {
    array[0] = 123;
    REQUIRE(123 == array[0].as<int>());
    REQUIRE(true == array[0].is<int>());
    REQUIRE(false == array[0].is<bool>());
  }

#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("long long") {
    array[0] = 9223372036854775807;
    REQUIRE(9223372036854775807 == array[0].as<int64_t>());
    REQUIRE(true == array[0].is<int64_t>());
    REQUIRE(false == array[0].is<int32_t>());
    REQUIRE(false == array[0].is<bool>());
  }
#endif

  SUBCASE("double") {
    array[0] = 123.45;
    REQUIRE(123.45 == array[0].as<double>());
    REQUIRE(true == array[0].is<double>());
    REQUIRE(false == array[0].is<int>());
  }

  SUBCASE("bool") {
    array[0] = true;
    REQUIRE(true == array[0].as<bool>());
    REQUIRE(true == array[0].is<bool>());
    REQUIRE(false == array[0].is<int>());
  }

  SUBCASE("string literal") {
    array[0] = "hello";

    REQUIRE(array[0].as<std::string>() == "hello");
    REQUIRE(true == array[0].is<const char*>());
    REQUIRE(false == array[0].is<int>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SUBCASE("const char*") {
    const char* str = "hello";
    array[0] = str;

    REQUIRE(array[0].as<std::string>() == "hello");
    REQUIRE(true == array[0].is<const char*>());
    REQUIRE(false == array[0].is<int>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

  SUBCASE("std::string") {
    array[0] = "hello"_s;

    REQUIRE(array[0].as<std::string>() == "hello");
    REQUIRE(true == array[0].is<const char*>());
    REQUIRE(false == array[0].is<int>());
    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                             Allocate(sizeofString("world")),
                         });
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SUBCASE("VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "world");

    array.add("hello");
    array[0] = vla;

    REQUIRE(array[0] == "world"_s);
  }
#endif

  SUBCASE("nested array") {
    JsonDocument doc2;
    JsonArray arr2 = doc2.to<JsonArray>();

    array[0] = arr2;

    REQUIRE(arr2 == array[0].as<JsonArray>());
    REQUIRE(true == array[0].is<JsonArray>());
    REQUIRE(false == array[0].is<int>());
  }

  SUBCASE("nested object") {
    JsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    array[0] = obj;

    REQUIRE(obj == array[0].as<JsonObject>());
    REQUIRE(true == array[0].is<JsonObject>());
    REQUIRE(false == array[0].is<int>());
  }

  SUBCASE("array subscript") {
    JsonDocument doc2;
    JsonArray arr2 = doc2.to<JsonArray>();
    const char* str = "hello";

    arr2.add(str);

    array[0] = arr2[0];

    REQUIRE(str == array[0]);
  }

  SUBCASE("object subscript") {
    const char* str = "hello";
    JsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    obj["x"] = str;

    array[0] = obj["x"];

    REQUIRE(str == array[0]);
  }

  SUBCASE("array[0].to<JsonObject>()") {
    JsonObject obj = array[0].to<JsonObject>();
    REQUIRE(obj.isNull() == false);
  }

  SUBCASE("Use a JsonVariant as index") {
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;

    REQUIRE(array[array[1]] == 3);
    REQUIRE(array[array[3]] == nullptr);
  }
}
