// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonArray::set()") {
  DynamicJsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  array.add(0);

  SECTION("int") {
    array.set(0, 123);
    REQUIRE(123 == array[0].as<int>());
    REQUIRE(array[0].is<int>());
    REQUIRE_FALSE(array[0].is<bool>());
  }

  SECTION("double") {
    array.set(0, 123.45);
    REQUIRE(123.45 == array[0].as<double>());
    REQUIRE(array[0].is<double>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("bool") {
    array.set(0, true);
    REQUIRE(true == array[0].as<bool>());
    REQUIRE(array[0].is<bool>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("const char*") {
    array.set(0, "hello");
    REQUIRE_THAT(array[0].as<const char*>(), Equals("hello"));
    REQUIRE(array[0].is<const char*>());
    REQUIRE_FALSE(array[0].is<int>());
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("set()") {
    int i = 16;
    char vla[i];
    strcpy(vla, "world");

    array.add("hello");
    array.set(0, vla);

    REQUIRE(std::string("world") == array[0]);
  }
#endif

  SECTION("nested array") {
    DynamicJsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();

    array.set(0, arr);

    REQUIRE(arr == array[0].as<JsonArray>());
    REQUIRE(array[0].is<JsonArray>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    array.set(0, obj);

    REQUIRE(obj == array[0].as<JsonObject>());
    REQUIRE(array[0].is<JsonObject>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("array subscript") {
    DynamicJsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();
    arr.add("hello");

    array.set(0, arr[0]);

    REQUIRE_THAT(array[0].as<char*>(), Equals("hello"));
  }

  SECTION("object subscript") {
    DynamicJsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();
    obj["x"] = "hello";

    array.set(0, obj["x"]);

    REQUIRE_THAT(array[0].as<char*>(), Equals("hello"));
  }

  SECTION("should not duplicate const char*") {
    array.set(0, "world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char*") {
    array.set(0, const_cast<char*>("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string") {
    array.set(0, std::string("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + 6;
    REQUIRE(expectedSize == doc.memoryUsage());
  }
}
