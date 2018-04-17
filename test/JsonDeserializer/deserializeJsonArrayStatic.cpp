// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize JSON array with a StaticJsonDocument") {
  SECTION("BufferOfTheRightSizeForEmptyArray") {
    StaticJsonDocument<JSON_ARRAY_SIZE(0)> doc;
    char input[] = "[]";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("TooSmallBufferForArrayWithOneValue") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1) - 1> doc;
    char input[] = "[1]";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("BufferOfTheRightSizeForArrayWithOneValue") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1)> doc;
    char input[] = "[1]";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("TooSmallBufferForArrayWithNestedObject") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) - 1> doc;
    char input[] = "[{}]";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("BufferOfTheRightSizeForArrayWithNestedObject") {
    StaticJsonDocument<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0)> doc;
    char input[] = "[{}]";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("CharPtrNull") {
    StaticJsonDocument<100> doc;

    JsonError err = deserializeJson(doc, static_cast<char*>(0));

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("ConstCharPtrNull") {
    StaticJsonDocument<100> doc;

    JsonError err = deserializeJson(doc, static_cast<const char*>(0));

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("CopyStringNotSpaces") {
    StaticJsonDocument<100> doc;

    deserializeJson(doc, "  [ \"1234567\" ] ");

    REQUIRE(JSON_ARRAY_SIZE(1) + sizeof("1234567") == doc.memoryUsage());
    // note: we use a string of 8 bytes to be sure that the StaticJsonBuffer
    // will not insert bytes to enforce alignement
  }

  SECTION("Should clear the JsonArray") {
    StaticJsonDocument<JSON_ARRAY_SIZE(4)> doc;
    char input[] = "[1,2,3,4]";

    deserializeJson(doc, input);
    deserializeJson(doc, "[]");

    JsonArray& arr = doc.as<JsonArray>();
    REQUIRE(arr.size() == 0);
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
  }

  SECTION("Array") {
    StaticJsonDocument<JSON_ARRAY_SIZE(2)> doc;
    char input[] = "[1,2]";

    JsonError err = deserializeJson(doc, input);
    JsonArray& arr = doc.as<JsonArray>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<JsonArray>());
    REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(2));
    REQUIRE(arr[0] == 1);
    REQUIRE(arr[1] == 2);
  }
}
