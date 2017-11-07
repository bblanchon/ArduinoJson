// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("StaticJsonBuffer::parseArray()") {
  SECTION("TooSmallBufferForEmptyArray") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(0) - 1> bufferTooSmall;
    char input[] = "[]";
    JsonArray& arr = bufferTooSmall.parseArray(input);
    REQUIRE_FALSE(arr.success());
  }

  SECTION("BufferOfTheRightSizeForEmptyArray") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(0)> bufferOfRightSize;
    char input[] = "[]";
    JsonArray& arr = bufferOfRightSize.parseArray(input);
    REQUIRE(arr.success());
  }

  SECTION("TooSmallBufferForArrayWithOneValue") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(1) - 1> bufferTooSmall;
    char input[] = "[1]";
    JsonArray& arr = bufferTooSmall.parseArray(input);
    REQUIRE_FALSE(arr.success());
  }

  SECTION("BufferOfTheRightSizeForArrayWithOneValue") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(1)> bufferOfRightSize;
    char input[] = "[1]";
    JsonArray& arr = bufferOfRightSize.parseArray(input);
    REQUIRE(arr.success());
  }

  SECTION("TooSmallBufferForArrayWithNestedObject") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) - 1>
        bufferTooSmall;
    char input[] = "[{}]";
    JsonArray& arr = bufferTooSmall.parseArray(input);
    REQUIRE_FALSE(arr.success());
  }

  SECTION("BufferOfTheRightSizeForArrayWithNestedObject") {
    StaticJsonBuffer<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0)>
        bufferOfRightSize;
    char input[] = "[{}]";
    JsonArray& arr = bufferOfRightSize.parseArray(input);
    REQUIRE(arr.success());
  }

  SECTION("CharPtrNull") {
    REQUIRE_FALSE(
        StaticJsonBuffer<100>().parseArray(static_cast<char*>(0)).success());
  }

  SECTION("ConstCharPtrNull") {
    REQUIRE_FALSE(StaticJsonBuffer<100>()
                      .parseArray(static_cast<const char*>(0))
                      .success());
  }

  SECTION("CopyStringNotSpaces") {
    StaticJsonBuffer<100> jsonBuffer;
    jsonBuffer.parseArray("  [ \"1234567\" ] ");
    REQUIRE(JSON_ARRAY_SIZE(1) + sizeof("1234567") == jsonBuffer.size());
    // note we use a string of 8 bytes to be sure that the StaticJsonBuffer
    // will not insert bytes to enforce alignement
  }
}
