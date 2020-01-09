// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize JSON array with a StaticJsonDocument") {
  SECTION("BufferOfTheRightSizeForEmptyArray") {
    StaticJsonDocument<JSON_ARRAY_SIZE(0)> doc;
    char input[] = "[]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("TooSmallBufferForArrayWithOneValue") {
    StaticJsonDocument<JSON_ARRAY_SIZE(0)> doc;
    char input[] = "[1]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SECTION("BufferOfTheRightSizeForArrayWithOneValue") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1)> doc;
    char input[] = "[1]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("TooSmallBufferForArrayWithNestedObject") {
    StaticJsonDocument<JSON_ARRAY_SIZE(0) + JSON_OBJECT_SIZE(0)> doc;
    char input[] = "[{}]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SECTION("BufferOfTheRightSizeForArrayWithNestedObject") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0)> doc;
    char input[] = "[{}]";

    DeserializationError err = deserializeJson(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("CopyStringNotSpaces") {
    StaticJsonDocument<100> doc;

    deserializeJson(doc, "  [ \"1234567\" ] ");

    REQUIRE(JSON_ARRAY_SIZE(1) + JSON_STRING_SIZE(8) == doc.memoryUsage());
    // note: we use a string of 8 bytes to be sure that the StaticMemoryPool
    // will not insert bytes to enforce alignement
  }

  SECTION("Should clear the JsonArray") {
    StaticJsonDocument<JSON_ARRAY_SIZE(4)> doc;
    char input[] = "[1,2,3,4]";

    deserializeJson(doc, input);
    deserializeJson(doc, "[]");

    JsonArray arr = doc.as<JsonArray>();
    REQUIRE(arr.size() == 0);
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
  }

  SECTION("Array") {
    StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;
    char input[] = "[1,2]";

    DeserializationError err = deserializeJson(doc, input);
    JsonArray arr = doc.as<JsonArray>();

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<JsonArray>());
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2));
    REQUIRE(arr[0] == 1);
    REQUIRE(arr[1] == 2);
  }
}
