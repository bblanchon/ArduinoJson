// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize MsgPack object") {
  DynamicJsonDocument doc;

  SECTION("fixmap") {
    SECTION("empty") {
      const char* input = "\x80";

      MsgPackError error = deserializeMsgPack(doc, input);
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(error == MsgPackError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 0);
    }

    SECTION("two integers") {
      const char* input = "\x82\xA3one\x01\xA3two\x02";

      MsgPackError error = deserializeMsgPack(doc, input);
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(error == MsgPackError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["one"] == 1);
      REQUIRE(obj["two"] == 2);
    }
  }

  SECTION("map 16") {
    SECTION("empty") {
      const char* input = "\xDE\x00\x00";

      MsgPackError error = deserializeMsgPack(doc, input);
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(error == MsgPackError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 0);
    }

    SECTION("two strings") {
      const char* input = "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world";

      MsgPackError error = deserializeMsgPack(doc, input);
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(error == MsgPackError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["H"] == "hello");
      REQUIRE(obj["W"] == "world");
    }
  }

  SECTION("map 32") {
    SECTION("empty") {
      const char* input = "\xDF\x00\x00\x00\x00";

      MsgPackError error = deserializeMsgPack(doc, input);
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(error == MsgPackError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 0);
    }

    SECTION("two floats") {
      const char* input =
          "\xDF\x00\x00\x00\x02\xA4zero\xCA\x00\x00\x00\x00\xA2pi\xCA\x40\x48"
          "\xF5\xC3";

      MsgPackError error = deserializeMsgPack(doc, input);
      JsonObject& obj = doc.as<JsonObject>();

      REQUIRE(error == MsgPackError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["zero"] == 0.0f);
      REQUIRE(obj["pi"] == 3.14f);
    }
  }
}
