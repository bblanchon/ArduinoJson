// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeJson(StaticJsonObject&)") {
  SECTION("BufferOfTheRightSizeForEmptyObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(0)> obj;
    char input[] = "{}";

    bool success = deserializeJson(obj, input);

    REQUIRE(success == true);
  }

  SECTION("TooSmallBufferForObjectWithOneValue") {
    StaticJsonObject<JSON_OBJECT_SIZE(1) - 1> obj;
    char input[] = "{\"a\":1}";

    bool success = deserializeJson(obj, input);

    REQUIRE(success == false);
  }

  SECTION("BufferOfTheRightSizeForObjectWithOneValue") {
    StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
    char input[] = "{\"a\":1}";

    bool success = deserializeJson(obj, input);

    REQUIRE(success == true);
  }

  SECTION("TooSmallBufferForObjectWithNestedObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0) - 1> obj;
    char input[] = "{\"a\":[]}";

    bool success = deserializeJson(obj, input);

    REQUIRE(success == false);
  }

  SECTION("BufferOfTheRightSizeForObjectWithNestedObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0)> obj;
    char input[] = "{\"a\":[]}";

    bool success = deserializeJson(obj, input);

    REQUIRE(success == true);
  }

  SECTION("CharPtrNull") {
    StaticJsonObject<100> obj;

    bool success = deserializeJson(obj, static_cast<char*>(0));

    REQUIRE(success == false);
  }

  SECTION("ConstCharPtrNull") {
    StaticJsonObject<100> obj;

    bool success = deserializeJson(obj, static_cast<const char*>(0));

    REQUIRE(success == false);
  }
}
