// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeJson(StaticJsonArray&)") {
  SECTION("BufferOfTheRightSizeForEmptyArray") {
    StaticJsonArray<JSON_ARRAY_SIZE(0)> arr;
    char input[] = "[]";

    bool success = deserializeJson(arr, input);

    REQUIRE(success == true);
  }

  SECTION("TooSmallBufferForArrayWithOneValue") {
    StaticJsonArray<JSON_ARRAY_SIZE(1) - 1> arr;
    char input[] = "[1]";

    bool success = deserializeJson(arr, input);

    REQUIRE(success == false);
  }

  SECTION("BufferOfTheRightSizeForArrayWithOneValue") {
    StaticJsonArray<JSON_ARRAY_SIZE(1)> arr;
    char input[] = "[1]";

    bool success = deserializeJson(arr, input);

    REQUIRE(success == true);
  }

  SECTION("TooSmallBufferForArrayWithNestedObject") {
    StaticJsonArray<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) - 1> arr;
    char input[] = "[{}]";

    bool success = deserializeJson(arr, input);

    REQUIRE(success == false);
  }

  SECTION("BufferOfTheRightSizeForArrayWithNestedObject") {
    StaticJsonArray<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0)> arr;
    char input[] = "[{}]";

    bool success = deserializeJson(arr, input);

    REQUIRE(success == true);
  }

  SECTION("CharPtrNull") {
    StaticJsonArray<100> arr;

    bool success = deserializeJson(arr, static_cast<char*>(0));

    REQUIRE(success == false);
  }

  SECTION("ConstCharPtrNull") {
    StaticJsonArray<100> arr;

    bool success = deserializeJson(arr, static_cast<const char*>(0));

    REQUIRE(success == false);
  }

  SECTION("CopyStringNotSpaces") {
    StaticJsonArray<100> arr;

    deserializeJson(arr, "  [ \"1234567\" ] ");

    REQUIRE(JSON_ARRAY_SIZE(1) + sizeof("1234567") == arr.memoryUsage());
    // note: we use a string of 8 bytes to be sure that the StaticJsonBuffer
    // will not insert bytes to enforce alignement
  }
}
