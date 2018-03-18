// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeJson(StaticJsonObject&)") {
  SECTION("BufferOfTheRightSizeForEmptyObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(0)> obj;
    char input[] = "{}";

    JsonError err = deserializeJson(obj, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("TooSmallBufferForObjectWithOneValue") {
    StaticJsonObject<JSON_OBJECT_SIZE(1) - 1> obj;
    char input[] = "{\"a\":1}";

    JsonError err = deserializeJson(obj, input);

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("BufferOfTheRightSizeForObjectWithOneValue") {
    StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
    char input[] = "{\"a\":1}";

    JsonError err = deserializeJson(obj, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("TooSmallBufferForObjectWithNestedObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0) - 1> obj;
    char input[] = "{\"a\":[]}";

    JsonError err = deserializeJson(obj, input);

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("BufferOfTheRightSizeForObjectWithNestedObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0)> obj;
    char input[] = "{\"a\":[]}";

    JsonError err = deserializeJson(obj, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("CharPtrNull") {
    StaticJsonObject<100> obj;

    JsonError err = deserializeJson(obj, static_cast<char*>(0));

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("ConstCharPtrNull") {
    StaticJsonObject<100> obj;

    JsonError err = deserializeJson(obj, static_cast<const char*>(0));

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("Should clear the JsonObject") {
    StaticJsonObject<JSON_OBJECT_SIZE(1)> obj;
    char input[] = "{\"hello\":\"world\"}";

    deserializeJson(obj, input);
    deserializeJson(obj, "{}");

    REQUIRE(obj.size() == 0);
    REQUIRE(obj.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
