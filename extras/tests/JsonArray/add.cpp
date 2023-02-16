// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::add()") {
  DynamicJsonDocument doc(4096);
  JsonArray array = doc.to<JsonArray>();

  SECTION("int") {
    array.add(123);
    REQUIRE(123 == array[0].as<int>());
    REQUIRE(array[0].is<int>());
    REQUIRE(array[0].is<double>());
  }

  SECTION("double") {
    array.add(123.45);
    REQUIRE(123.45 == array[0].as<double>());
    REQUIRE(array[0].is<double>());
    REQUIRE_FALSE(array[0].is<bool>());
  }

  SECTION("bool") {
    array.add(true);
    REQUIRE(true == array[0].as<bool>());
    REQUIRE(array[0].is<bool>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("const char*") {
    const char* str = "hello";
    array.add(str);
    REQUIRE(str == array[0].as<std::string>());
    REQUIRE(array[0].is<const char*>());
    REQUIRE_FALSE(array[0].is<int>());
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("vla") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "world");

    array.add(vla);

    REQUIRE(std::string("world") == array[0]);
  }
#endif

  SECTION("nested array") {
    DynamicJsonDocument doc2(4096);
    JsonArray arr = doc2.to<JsonArray>();

    array.add(arr);

    REQUIRE(arr == array[0].as<JsonArray>());
    REQUIRE(array[0].is<JsonArray>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("nested object") {
    DynamicJsonDocument doc2(4096);
    JsonObject obj = doc2.to<JsonObject>();

    array.add(obj);

    REQUIRE(obj == array[0].as<JsonObject>());
    REQUIRE(array[0].is<JsonObject>());
    REQUIRE_FALSE(array[0].is<int>());
  }

  SECTION("array subscript") {
    const char* str = "hello";
    DynamicJsonDocument doc2(4096);
    JsonArray arr = doc2.to<JsonArray>();
    arr.add(str);

    array.add(arr[0]);

    REQUIRE(str == array[0]);
  }

  SECTION("object subscript") {
    const char* str = "hello";
    DynamicJsonDocument doc2(4096);
    JsonObject obj = doc2.to<JsonObject>();
    obj["x"] = str;

    array.add(obj["x"]);

    REQUIRE(str == array[0]);
  }

  SECTION("should not duplicate const char*") {
    array.add("world");
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate char*") {
    array.add(const_cast<char*>("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + JSON_STRING_SIZE(5);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate std::string") {
    array.add(std::string("world"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + JSON_STRING_SIZE(5);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should not duplicate serialized(const char*)") {
    array.add(serialized("{}"));
    const size_t expectedSize = JSON_ARRAY_SIZE(1);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate serialized(char*)") {
    array.add(serialized(const_cast<char*>("{}")));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + JSON_STRING_SIZE(2);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate serialized(std::string)") {
    array.add(serialized(std::string("{}")));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + JSON_STRING_SIZE(2);
    REQUIRE(expectedSize == doc.memoryUsage());
  }

  SECTION("should duplicate serialized(std::string)") {
    array.add(serialized(std::string("\0XX", 3)));
    const size_t expectedSize = JSON_ARRAY_SIZE(1) + JSON_STRING_SIZE(3);
    REQUIRE(expectedSize == doc.memoryUsage());
  }
}
