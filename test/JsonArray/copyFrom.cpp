// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonArray::copyFrom()") {
  SECTION("OneDimension") {
    DynamicJsonDocument doc;
    JsonArray& array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = array.copyFrom(source);
    REQUIRE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[1,2,3]") == json);
  }

  SECTION("OneDimension_JsonBufferTooSmall") {
    const size_t SIZE = JSON_ARRAY_SIZE(2);
    StaticJsonDocument<SIZE> doc;
    JsonArray& array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = array.copyFrom(source);
    REQUIRE_FALSE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[1,2]") == json);
  }

  SECTION("TwoDimensions") {
    DynamicJsonDocument doc;
    JsonArray& array = doc.to<JsonArray>();
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = array.copyFrom(source);
    REQUIRE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("TwoDimensions_JsonBufferTooSmall") {
    const size_t SIZE =
        JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(2);
    StaticJsonDocument<SIZE> doc;
    JsonArray& array = doc.to<JsonArray>();
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = array.copyFrom(source);
    REQUIRE_FALSE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[[1,2,3],[4,5]]") == json);
  }
}
