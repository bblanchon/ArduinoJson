// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonArray::operator[]") {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array = _jsonBuffer.createArray();
  _array.add(0);

  SECTION("SizeIsUnchanged") {
    _array[0] = "hello";
    REQUIRE(1U == _array.size());
  }

  SECTION("int") {
    _array[0] = 123;
    REQUIRE(123 == _array[0].as<int>());
    REQUIRE(true == _array[0].is<int>());
    REQUIRE(false == _array[0].is<bool>());
  }

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("long long") {
    _array[0] = 9223372036854775807;
    REQUIRE(9223372036854775807 == _array[0].as<long long>());
    REQUIRE(true == _array[0].is<int>());
    REQUIRE(false == _array[0].is<bool>());
  }
#endif

  SECTION("double") {
    _array[0] = 123.45;
    REQUIRE(123.45 == _array[0].as<double>());
    REQUIRE(true == _array[0].is<double>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("bool") {
    _array[0] = true;
    REQUIRE(true == _array[0].as<bool>());
    REQUIRE(true == _array[0].is<bool>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("const char*") {
    const char* str = "hello";

    _array[0] = str;
    REQUIRE(str == _array[0].as<const char*>());
    REQUIRE(str == _array[0].as<char*>());  // <- short hand
    REQUIRE(true == _array[0].is<const char*>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("nested array") {
    JsonArray& arr = _jsonBuffer.createArray();

    _array[0] = arr;

    REQUIRE(&arr == &_array[0].as<JsonArray&>());
    REQUIRE(&arr == &_array[0].as<JsonArray>());  // <- short hand
    REQUIRE(&arr == &_array[0].as<const JsonArray&>());
    REQUIRE(&arr == &_array[0].as<const JsonArray>());  // <- short hand
    REQUIRE(true == _array[0].is<JsonArray&>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("nested object") {
    JsonObject& obj = _jsonBuffer.createObject();

    _array[0] = obj;

    REQUIRE(&obj == &_array[0].as<JsonObject&>());
    REQUIRE(&obj == &_array[0].as<JsonObject>());  // <- short hand
    REQUIRE(&obj == &_array[0].as<const JsonObject&>());
    REQUIRE(&obj == &_array[0].as<const JsonObject>());  // <- short hand
    REQUIRE(true == _array[0].is<JsonObject&>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("array subscript") {
    JsonArray& arr = _jsonBuffer.createArray();
    const char* str = "hello";

    arr.add(str);

    _array[0] = arr[0];

    REQUIRE(str == _array[0]);
  }

  SECTION("object subscript") {
    JsonObject& obj = _jsonBuffer.createObject();
    const char* str = "hello";

    obj["x"] = str;

    _array[0] = obj["x"];

    REQUIRE(str == _array[0]);
  }
}
