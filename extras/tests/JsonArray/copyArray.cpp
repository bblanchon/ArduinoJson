// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("copyArray()") {
  SECTION("1D -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, array);
    REQUIRE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[1,2,3]") == json);
  }

  SECTION("1D -> JsonDocument") {
    DynamicJsonDocument doc(4096);
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, doc);
    REQUIRE(ok);

    serializeJson(doc, json, sizeof(json));
    REQUIRE(std::string("[1,2,3]") == json);
  }

  SECTION("1D -> JsonArray, but not enough memory") {
    const size_t SIZE = JSON_ARRAY_SIZE(2);
    StaticJsonDocument<SIZE> doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, array);
    REQUIRE_FALSE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[1,2]") == json);
  }

  SECTION("2D -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, array);
    REQUIRE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("2D -> JsonDocument") {
    DynamicJsonDocument doc(4096);
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, doc);
    REQUIRE(ok);

    serializeJson(doc, json, sizeof(json));
    REQUIRE(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("2D -> JsonArray, but not enough memory") {
    const size_t SIZE =
        JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(2);
    StaticJsonDocument<SIZE> doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32] = "";
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    CAPTURE(SIZE)

    bool ok = copyArray(source, array);
    CAPTURE(doc.memoryUsage());
    CHECK_FALSE(ok);

    serializeJson(array, json, sizeof(json));
    REQUIRE(std::string("[[1,2,3],[4,5]]") == json);
  }

  SECTION("JsonArray -> 1D, with more space than needed") {
    DynamicJsonDocument doc(4096);
    char json[] = "[1,2,3]";
    DeserializationError err = deserializeJson(doc, json);
    REQUIRE(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    int destination[4] = {0};
    size_t result = copyArray(array, destination);

    REQUIRE(3 == result);
    REQUIRE(1 == destination[0]);
    REQUIRE(2 == destination[1]);
    REQUIRE(3 == destination[2]);
    REQUIRE(0 == destination[3]);
  }

  SECTION("JsonArray -> 1D, without enough space") {
    DynamicJsonDocument doc(4096);
    char json[] = "[1,2,3]";
    DeserializationError err = deserializeJson(doc, json);
    REQUIRE(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    int destination[2] = {0};
    size_t result = copyArray(array, destination);

    REQUIRE(2 == result);
    REQUIRE(1 == destination[0]);
    REQUIRE(2 == destination[1]);
  }

  SECTION("JsonDocument -> 1D") {
    DynamicJsonDocument doc(4096);
    char json[] = "[1,2,3]";
    DeserializationError err = deserializeJson(doc, json);
    REQUIRE(err == DeserializationError::Ok);

    int destination[4] = {0};
    size_t result = copyArray(doc, destination);

    REQUIRE(3 == result);
    REQUIRE(1 == destination[0]);
    REQUIRE(2 == destination[1]);
    REQUIRE(3 == destination[2]);
    REQUIRE(0 == destination[3]);
  }

  SECTION("JsonArray -> 2D") {
    DynamicJsonDocument doc(4096);
    char json[] = "[[1,2],[3],[4]]";

    DeserializationError err = deserializeJson(doc, json);
    REQUIRE(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    int destination[3][2] = {{0}};
    copyArray(array, destination);

    REQUIRE(1 == destination[0][0]);
    REQUIRE(2 == destination[0][1]);
    REQUIRE(3 == destination[1][0]);
    REQUIRE(0 == destination[1][1]);
    REQUIRE(4 == destination[2][0]);
    REQUIRE(0 == destination[2][1]);
  }

  SECTION("JsonDocument -> 2D") {
    DynamicJsonDocument doc(4096);
    char json[] = "[[1,2],[3],[4]]";

    DeserializationError err = deserializeJson(doc, json);
    REQUIRE(err == DeserializationError::Ok);

    int destination[3][2] = {{0}};
    copyArray(doc, destination);

    REQUIRE(1 == destination[0][0]);
    REQUIRE(2 == destination[0][1]);
    REQUIRE(3 == destination[1][0]);
    REQUIRE(0 == destination[1][1]);
    REQUIRE(4 == destination[2][0]);
    REQUIRE(0 == destination[2][1]);
  }
}
