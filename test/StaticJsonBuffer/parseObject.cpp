// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
TEST_CASE("StaticJsonBuffer::parseObject()") {
  SECTION("TooSmallBufferForEmptyObject") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(0) - 1> bufferTooSmall;
    char input[] = "{}";
    JsonObject& obj = bufferTooSmall.parseObject(input);
    REQUIRE_FALSE(obj.success());
  }

  SECTION("BufferOfTheRightSizeForEmptyObject") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(0)> bufferOfRightSize;
    char input[] = "{}";
    JsonObject& obj = bufferOfRightSize.parseObject(input);
    REQUIRE(obj.success());
  }

  SECTION("TooSmallBufferForObjectWithOneValue") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) - 1> bufferTooSmall;
    char input[] = "{\"a\":1}";
    JsonObject& obj = bufferTooSmall.parseObject(input);
    REQUIRE_FALSE(obj.success());
  }

  SECTION("BufferOfTheRightSizeForObjectWithOneValue") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1)> bufferOfRightSize;
    char input[] = "{\"a\":1}";
    JsonObject& obj = bufferOfRightSize.parseObject(input);
    REQUIRE(obj.success());
  }

  SECTION("TooSmallBufferForObjectWithNestedObject") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0) - 1>
        bufferTooSmall;
    char input[] = "{\"a\":[]}";
    JsonObject& obj = bufferTooSmall.parseObject(input);
    REQUIRE_FALSE(obj.success());
  }

  SECTION("BufferOfTheRightSizeForObjectWithNestedObject") {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0)>
        bufferOfRightSize;
    char input[] = "{\"a\":[]}";
    JsonObject& obj = bufferOfRightSize.parseObject(input);
    REQUIRE(obj.success());
  }

  SECTION("CharPtrNull") {
    REQUIRE_FALSE(
        StaticJsonBuffer<100>().parseObject(static_cast<char*>(0)).success());
  }

  SECTION("ConstCharPtrNull") {
    REQUIRE_FALSE(StaticJsonBuffer<100>()
                      .parseObject(static_cast<const char*>(0))
                      .success());
  }
}
