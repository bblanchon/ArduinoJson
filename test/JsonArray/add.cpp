// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::add()") {
  DynamicJsonDocument doc;
  JsonArray& _array = doc.to<JsonArray>();

  SECTION("int") {
    _array.add(123);
    REQUIRE(123 == _array[0].as<int>());
    REQUIRE(_array[0].is<int>());
    REQUIRE(_array[0].is<double>());
  }

  SECTION("double") {
    _array.add(123.45);
    REQUIRE(123.45 == _array[0].as<double>());
    REQUIRE(_array[0].is<double>());
    REQUIRE_FALSE(_array[0].is<bool>());
  }

  SECTION("bool") {
    _array.add(true);
    REQUIRE(true == _array[0].as<bool>());
    REQUIRE(_array[0].is<bool>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("const char*") {
    const char* str = "hello";
    _array.add(str);
    REQUIRE(str == _array[0].as<std::string>());
    REQUIRE(_array[0].is<const char*>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("nested array") {
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();

    _array.add(arr);

    REQUIRE(&arr == &_array[0].as<JsonArray&>());
    REQUIRE(_array[0].is<JsonArray&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2;
    JsonObject& obj = doc2.to<JsonObject>();

    _array.add(obj);

    REQUIRE(&obj == &_array[0].as<JsonObject&>());
    REQUIRE(_array[0].is<JsonObject&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("array subscript") {
    const char* str = "hello";
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();
    arr.add(str);

    _array.add(arr[0]);

    REQUIRE(str == _array[0]);
  }

  SECTION("object subscript") {
    const char* str = "hello";
    DynamicJsonDocument doc2;
    JsonObject& obj = doc2.to<JsonObject>();
    obj["x"] = str;

    _array.add(obj["x"]);

    REQUIRE(str == _array[0]);
  }

  SECTION("should not duplicate const char*") {
    _array.add("world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char*") {
    _array.add(const_cast<char*>("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string") {
    _array.add(std::string("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should not duplicate RawJson(const char*)") {
    _array.add(RawJson("{}"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate RawJson(char*)") {
    _array.add(RawJson(const_cast<char*>("{}")));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 3;
    REQUIRE(expectedSize == doc.memoryUsage());
  }
}
