// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize MsgPack array") {
  DynamicJsonDocument doc(4096);

  SECTION("fixarray") {
    SECTION("empty") {
      const char* input = "\x90";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 0);
    }

    SECTION("two integers") {
      const char* input = "\x92\x01\x02";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == 1);
      REQUIRE(array[1] == 2);
    }
  }

  SECTION("array 16") {
    SECTION("empty") {
      const char* input = "\xDC\x00\x00";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 0);
    }

    SECTION("two strings") {
      const char* input = "\xDC\x00\x02\xA5hello\xA5world";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == "hello");
      REQUIRE(array[1] == "world");
    }
  }

  SECTION("array 32") {
    SECTION("empty") {
      const char* input = "\xDD\x00\x00\x00\x00";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 0);
    }

    SECTION("two floats") {
      const char* input =
          "\xDD\x00\x00\x00\x02\xCA\x00\x00\x00\x00\xCA\x40\x48\xF5\xC3";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == 0.0f);
      REQUIRE(array[1] == 3.14f);
    }
  }
}
