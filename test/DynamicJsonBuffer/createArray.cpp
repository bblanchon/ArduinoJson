// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonBuffer::createArray()") {
  DynamicJsonBuffer jsonBuffer;
  JsonArray &array = jsonBuffer.createArray();

  SECTION("GrowsWithArray") {
    REQUIRE(JSON_ARRAY_SIZE(0) == jsonBuffer.size());

    array.add("hello");
    REQUIRE(JSON_ARRAY_SIZE(1) == jsonBuffer.size());

    array.add("world");
    REQUIRE(JSON_ARRAY_SIZE(2) == jsonBuffer.size());
  }

  SECTION("CanAdd1000Values") {
    for (size_t i = 1; i <= 1000; i++) {
      array.add("hello");
      REQUIRE(array.size() == i);
    }
  }
}
