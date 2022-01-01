// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("copyArray()") {
  SECTION("int[] -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[1,2,3]") == json);
  }

  SECTION("std::string[] -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    std::string source[] = {"a", "b", "c"};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char*[] -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    const char* source[] = {"a", "b", "c"};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char[][] -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    char source[][2] = {"a", "b", "c"};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char[][] -> JsonDocument") {
    DynamicJsonDocument doc(4096);
    char json[32];
    char source[][2] = {"a", "b", "c"};

    bool ok = copyArray(source, doc);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("[\"a\",\"b\",\"c\"]") == json);
  }

  SECTION("const char[][] -> MemberProxy") {
    DynamicJsonDocument doc(4096);
    char json[32];
    char source[][2] = {"a", "b", "c"};

    bool ok = copyArray(source, doc["data"]);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("{\"data\":[\"a\",\"b\",\"c\"]}") == json);
  }

  SECTION("int[] -> JsonDocument") {
    DynamicJsonDocument doc(4096);
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, doc);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("[1,2,3]") == json);
  }

  SECTION("int[] -> MemberProxy") {
    DynamicJsonDocument doc(4096);
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, doc["data"]);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("{\"data\":[1,2,3]}") == json);
  }

  SECTION("int[] -> JsonArray, but not enough memory") {
    const size_t SIZE = JSON_ARRAY_SIZE(2);
    StaticJsonDocument<SIZE> doc;
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[] = {1, 2, 3};

    bool ok = copyArray(source, array);
    REQUIRE_FALSE(ok);

    serializeJson(array, json);
    CHECK(std::string("[1,2]") == json);
  }

  SECTION("int[][] -> JsonArray") {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, array);
    CHECK(ok);

    serializeJson(array, json);
    CHECK(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("int[][] -> MemberProxy") {
    DynamicJsonDocument doc(4096);
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, doc["data"]);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("{\"data\":[[1,2,3],[4,5,6]]}") == json);
  }

  SECTION("int[][] -> JsonDocument") {
    DynamicJsonDocument doc(4096);
    char json[32];
    int source[][3] = {{1, 2, 3}, {4, 5, 6}};

    bool ok = copyArray(source, doc);
    CHECK(ok);

    serializeJson(doc, json);
    CHECK(std::string("[[1,2,3],[4,5,6]]") == json);
  }

  SECTION("int[][] -> JsonArray, but not enough memory") {
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

    serializeJson(array, json);
    CHECK(std::string("[[1,2,3],[4,5]]") == json);
  }

  SECTION("JsonArray -> int[], with more space than needed") {
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
    DynamicJsonDocument doc(4096);
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
