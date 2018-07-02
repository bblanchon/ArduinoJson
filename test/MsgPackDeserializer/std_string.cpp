// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserializeMsgPack(const std::string&)") {
  DynamicJsonDocument doc;

  SECTION("should accept const string") {
    const std::string input("\x92\x01\x02");

    DeserializationError err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("should accept temporary string") {
    DeserializationError err =
        deserializeMsgPack(doc, std::string("\x92\x01\x02"));

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("should duplicate content") {
    std::string input("\x91\xA5hello");

    DeserializationError err = deserializeMsgPack(doc, input);
    input[2] = 'X';  // alter the string tomake sure we made a copy

    JsonArray array = doc.as<JsonArray>();
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(std::string("hello") == array[0]);
  }

  SECTION("should accept a zero in input") {
    DeserializationError err =
        deserializeMsgPack(doc, std::string("\x92\x00\x02", 3));

    REQUIRE(err == DeserializationError::Ok);
    JsonArray arr = doc.as<JsonArray>();
    REQUIRE(arr[0] == 0);
    REQUIRE(arr[1] == 2);
  }
}
