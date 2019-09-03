// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("serialize MsgPack to various destination types") {
  DynamicJsonDocument doc(4096);
  JsonObject object = doc.to<JsonObject>();
  object["hello"] = "world";
  const char *expected_result = "\x81\xA5hello\xA5world";
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

  SECTION("char[]") {
    char result[64];
    size_t len = serializeMsgPack(object, result);

    REQUIRE(std::string(expected_result) == result);
    REQUIRE(expected_length == len);
  }

  SECTION("char*") {
    char result[64];
    size_t len = serializeMsgPack(object, result, 64);

    REQUIRE(std::string(expected_result) == result);
    REQUIRE(expected_length == len);
  }
}
