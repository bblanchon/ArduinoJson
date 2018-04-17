// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonArray::set()") {
  DynamicJsonDocument doc;
  JsonArray& _array = doc.to<JsonArray>();
  _array.add(0);

  SECTION("int") {
    _array.set(0, 123);
    REQUIRE(123 == _array[0].as<int>());
    REQUIRE(_array[0].is<int>());
    REQUIRE_FALSE(_array[0].is<bool>());
  }

  SECTION("double") {
    _array.set(0, 123.45);
    REQUIRE(123.45 == _array[0].as<double>());
    REQUIRE(_array[0].is<double>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("bool") {
    _array.set(0, true);
    REQUIRE(true == _array[0].as<bool>());
    REQUIRE(_array[0].is<bool>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("const char*") {
    _array.set(0, "hello");
    REQUIRE_THAT(_array[0].as<const char*>(), Equals("hello"));
    REQUIRE(_array[0].is<const char*>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("nested array") {
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();

    _array.set(0, arr);

    REQUIRE(&arr == &_array[0].as<JsonArray&>());
    REQUIRE(_array[0].is<JsonArray&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2;
    JsonObject& obj = doc2.to<JsonObject>();

    _array.set(0, obj);

    REQUIRE(&obj == &_array[0].as<JsonObject&>());
    REQUIRE(_array[0].is<JsonObject&>());
    REQUIRE_FALSE(_array[0].is<int>());
  }

  SECTION("array subscript") {
    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();
    arr.add("hello");

    _array.set(0, arr[0]);

    REQUIRE_THAT(_array[0].as<char*>(), Equals("hello"));
  }

  SECTION("object subscript") {
    DynamicJsonDocument doc2;
    JsonObject& obj = doc2.to<JsonObject>();
    obj["x"] = "hello";

    _array.set(0, obj["x"]);

    REQUIRE_THAT(_array[0].as<char*>(), Equals("hello"));
  }

  SECTION("should not duplicate const char*") {
    _array.set(0, "world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char*") {
    _array.set(0, const_cast<char*>("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string") {
    _array.set(0, std::string("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }
}
