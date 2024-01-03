// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("copyArray()") {
  SECTION("int[] -> JsonArray") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[1,2,3]") == json);
  }

  SECTION("std::string[] -> JsonArray") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    std::string source[] = {"a", "b", "c"};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char*[] -> JsonArray") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    const char* source[] = {"a", "b", "c"};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char[][] -> JsonArray") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    char source[][2] = {"a", "b", "c"};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char[][] -> JsonDocument") {
    JsonDocument doc;
    char json[32];
    char source[][2] = {"a", "b", "c"};

    bool ok = copyArray(source, doc);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char[][] -> MemberProxy") {
    JsonDocument doc;
    char json[32];
    char source[][2] = {"a", "b", "c"};

    bool ok = copyArray(source, doc["data"]);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("{\"data\":[\"a\",\"b\",\"c\"]}") == json);
  }

  SECTION("int[] -> JsonDocument") {
    JsonDocument doc;
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, doc);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("[1,2,3]") == json);
  }

  SECTION("int[] -> MemberProxy") {
    JsonDocument doc;
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, doc["data"]);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("{\"data\":[1,2,3]}") == json);
  }

  SECTION("int[] -> JsonArray, but not enough memory") {
    JsonDocument doc(FailingAllocator::instance());
    JsonArray array = doc.to<JsonArray>();
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, array);
    REQUIRE_FALSE(ok);
  }

  SECTION("int[][] -> JsonArray") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("int[][] -> MemberProxy") {
    JsonDocument doc;
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, doc["data"]);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("{\"data\":[[1,2,3],[4,5,6]]}") == json);
  }

  SECTION("int[][] -> JsonDocument") {
    JsonDocument doc;
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, doc);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("int[][] -> JsonArray, but not enough memory") {
    JsonDocument doc(FailingAllocator::instance());
    JsonArray array = doc.to<JsonArray>();
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, array);
    REQUIRE(ok == false);
  }

  SECTION("JsonArray -> int[], with more space than needed") {
    JsonDocument doc;
    char json[] = "[1,2,3]";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    int destination[4] = {0};
    size_t result = copyArray(array, destination);

    CHECK(3 == result);
    CHECK(1 == destination[0]);
    CHECK(2 == destination[1]);
    CHECK(3 == destination[2]);
    CHECK(0 == destination[3]);
  }

  SECTION("JsonArray -> int[], without enough space") {
    JsonDocument doc;
    char json[] = "[1,2,3]";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    int destination[2] = {0};
    size_t result = copyArray(array, destination);

    CHECK(2 == result);
    CHECK(1 == destination[0]);
    CHECK(2 == destination[1]);
  }

  SECTION("JsonArray -> std::string[]") {
    JsonDocument doc;
    char json[] = "[\"a\",\"b\",\"c\"]";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    std::string destination[4];
    size_t result = copyArray(array, destination);

    CHECK(3 == result);
    CHECK("a" == destination[0]);
    CHECK("b" == destination[1]);
    CHECK("c" == destination[2]);
    CHECK("" == destination[3]);
  }

  SECTION("JsonArray -> char[N][]") {
    JsonDocument doc;
    char json[] = "[\"a12345\",\"b123456\",\"c1234567\"]";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    char destination[4][8] = {{0}};
    size_t result = copyArray(array, destination);

    CHECK(3 == result);
    CHECK(std::string("a12345") == destination[0]);
    CHECK(std::string("b123456") == destination[1]);
    CHECK(std::string("c123456") == destination[2]);  // truncated
    CHECK(std::string("") == destination[3]);
  }

  SECTION("JsonDocument -> int[]") {
    JsonDocument doc;
    char json[] = "[1,2,3]";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);

    int destination[4] = {0};
    size_t result = copyArray(doc, destination);

    CHECK(3 == result);
    CHECK(1 == destination[0]);
    CHECK(2 == destination[1]);
    CHECK(3 == destination[2]);
    CHECK(0 == destination[3]);
  }

  SECTION("MemberProxy -> int[]") {
    JsonDocument doc;
    char json[] = "{\"data\":[1,2,3]}";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);

    int destination[4] = {0};
    size_t result = copyArray(doc["data"], destination);

    CHECK(3 == result);
    CHECK(1 == destination[0]);
    CHECK(2 == destination[1]);
    CHECK(3 == destination[2]);
    CHECK(0 == destination[3]);
  }

  SECTION("ElementProxy -> int[]") {
    JsonDocument doc;
    char json[] = "[[1,2,3]]";
    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);

    int destination[4] = {0};
    size_t result = copyArray(doc[0], destination);

    CHECK(3 == result);
    CHECK(1 == destination[0]);
    CHECK(2 == destination[1]);
    CHECK(3 == destination[2]);
    CHECK(0 == destination[3]);
  }

  SECTION("JsonArray -> int[][]") {
    JsonDocument doc;
    char json[] = "[[1,2],[3],[4]]";

    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);
    JsonArray array = doc.as<JsonArray>();

    int destination[3][2] = {{0}};
    copyArray(array, destination);

    CHECK(1 == destination[0][0]);
    CHECK(2 == destination[0][1]);
    CHECK(3 == destination[1][0]);
    CHECK(0 == destination[1][1]);
    CHECK(4 == destination[2][0]);
    CHECK(0 == destination[2][1]);
  }

  SECTION("JsonDocument -> int[][]") {
    JsonDocument doc;
    char json[] = "[[1,2],[3],[4]]";

    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);

    int destination[3][2] = {{0}};
    copyArray(doc, destination);

    CHECK(1 == destination[0][0]);
    CHECK(2 == destination[0][1]);
    CHECK(3 == destination[1][0]);
    CHECK(0 == destination[1][1]);
    CHECK(4 == destination[2][0]);
    CHECK(0 == destination[2][1]);
  }

  SECTION("MemberProxy -> int[][]") {
    JsonDocument doc;
    char json[] = "{\"data\":[[1,2],[3],[4]]}";

    DeserializationError err = deserializeJson(doc, json);
    CHECK(err == DeserializationError::Ok);

    int destination[3][2] = {{0}};
    copyArray(doc["data"], destination);

    CHECK(1 == destination[0][0]);
    CHECK(2 == destination[0][1]);
    CHECK(3 == destination[1][0]);
    CHECK(0 == destination[1][1]);
    CHECK(4 == destination[2][0]);
    CHECK(0 == destination[2][1]);
  }
}
