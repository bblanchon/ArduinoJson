// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::size()") {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array = _jsonBuffer.createArray();

  SECTION("increases after add()") {
    _array.add("hello");
    REQUIRE(1U == _array.size());

    _array.add("world");
    REQUIRE(2U == _array.size());
  }

  SECTION("remains the same after set()") {
    _array.add("hello");
    REQUIRE(1U == _array.size());

    _array.set(0, "hello");
    REQUIRE(1U == _array.size());
  }

  SECTION("remains the same after assigment") {
    _array.add("hello");
    REQUIRE(1U == _array.size());

    _array[0] = "hello";
    REQUIRE(1U == _array.size());
  }
}
