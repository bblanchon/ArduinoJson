// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonArray::operator[]") {
  DynamicJsonDocument doc;
  JsonArray& _array = doc.to<JsonArray>();
  _array.add(0);

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
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();

    _array[0] = arr;

    REQUIRE(&arr == &_array[0].as<JsonArray&>());
    REQUIRE(&arr == &_array[0].as<JsonArray>());  // <- short hand
    REQUIRE(&arr == &_array[0].as<const JsonArray&>());
    REQUIRE(&arr == &_array[0].as<const JsonArray>());  // <- short hand
    REQUIRE(true == _array[0].is<JsonArray&>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2;
    JsonObject& obj = doc2.to<JsonObject>();

    _array[0] = obj;

    REQUIRE(&obj == &_array[0].as<JsonObject&>());
    REQUIRE(&obj == &_array[0].as<JsonObject>());  // <- short hand
    REQUIRE(&obj == &_array[0].as<const JsonObject&>());
    REQUIRE(&obj == &_array[0].as<const JsonObject>());  // <- short hand
    REQUIRE(true == _array[0].is<JsonObject&>());
    REQUIRE(false == _array[0].is<int>());
  }

  SECTION("array subscript") {
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();
    const char* str = "hello";

    arr.add(str);

    _array[0] = arr[0];

    REQUIRE(str == _array[0]);
  }

  SECTION("object subscript") {
    const char* str = "hello";
    DynamicJsonDocument doc2;
    JsonObject& obj = doc2.to<JsonObject>();

    obj["x"] = str;

    _array[0] = obj["x"];

    REQUIRE(str == _array[0]);
  }

  SECTION("should not duplicate const char*") {
    _array[0] = "world";
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char*") {
    _array[0] = const_cast<char*>("world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string") {
    _array[0] = std::string("world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }
}
