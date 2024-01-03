// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("serialize MsgPack to various destination types") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();
  object["hello"] = "world";
  const char* expected_result = "\x81\xA5hello\xA5world";
  const size_t expected_length = 13;

  SECTION("std::string") {
    std::string result;
    size_t len = serializeMsgPack(object, result);

    REQUIRE(expected_result == result);
    REQUIRE(expected_length == len);
  }

  /*  SECTION("std::vector<char>") {
      std::vector<char> result;
      size_t len = serializeMsgPack(object, result);

      REQUIRE(std::vector<char>(expected_result, expected_result + 13) ==
    result);
    REQUIRE(expected_length == len);
    } */

  SECTION("char[] larger than needed") {
    char result[64];
    memset(result, 42, sizeof(result));
    size_t len = serializeMsgPack(object, result);

    REQUIRE(expected_length == len);
    REQUIRE(std::string(expected_result, len) == std::string(result, len));
    REQUIRE(result[len] == 42);
  }

  SECTION("char[] of the right size") {  // #1545
    char result[13];
    size_t len = serializeMsgPack(object, result);

    REQUIRE(expected_length == len);
    REQUIRE(std::string(expected_result, len) == std::string(result, len));
  }

  SECTION("char*") {
    char result[64];
    memset(result, 42, sizeof(result));
    size_t len = serializeMsgPack(object, result, 64);

    REQUIRE(expected_length == len);
    REQUIRE(std::string(expected_result, len) == std::string(result, len));
    REQUIRE(result[len] == 42);
  }
}
