// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonBuffer::startString()") {
  SECTION("WorksWhenBufferIsBigEnough") {
    DynamicJsonBuffer jsonBuffer(6);

    DynamicJsonBuffer::String str = jsonBuffer.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(std::string("hello") == str.c_str());
  }

  SECTION("GrowsWhenBufferIsTooSmall") {
    DynamicJsonBuffer jsonBuffer(5);

    DynamicJsonBuffer::String str = jsonBuffer.startString();
    str.append('h');
    str.append('e');
    str.append('l');
    str.append('l');
    str.append('o');

    REQUIRE(std::string("hello") == str.c_str());
  }

  SECTION("SizeIncreases") {
    DynamicJsonBuffer jsonBuffer(5);

    DynamicJsonBuffer::String str = jsonBuffer.startString();
    REQUIRE(0 == jsonBuffer.size());

    str.append('h');
    REQUIRE(1 == jsonBuffer.size());

    str.c_str();
    REQUIRE(2 == jsonBuffer.size());
  }
}
