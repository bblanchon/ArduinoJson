// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize JSON object with StaticJsonDocument") {
  SECTION("BufferOfTheRightSizeForEmptyObject") {
    StaticJsonDocument<JSON_OBJECT_SIZE(0)> doc;
    char input[] = "{}";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("TooSmallBufferForObjectWithOneValue") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1) - 1> doc;
    char input[] = "{\"a\":1}";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("BufferOfTheRightSizeForObjectWithOneValue") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    char input[] = "{\"a\":1}";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err == JsonError::Ok);
  }

  SECTION("TooSmallBufferForObjectWithNestedObject") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0) - 1> doc;
    char input[] = "{\"a\":[]}";

    JsonError err = deserializeJson(doc, input);

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("BufferOfTheRightSizeForObjectWithNestedObject") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0)> doc;
    char input[] = "{\"a\":[]}";

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

  SECTION("Should clear the JsonObject") {
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    char input[] = "{\"hello\":\"world\"}";

    deserializeJson(doc, input);
    deserializeJson(doc, "{}");

    REQUIRE(doc.as<JsonObject>().size() == 0);
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
  }
}
