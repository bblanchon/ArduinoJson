// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("deserialize MsgPack object") {
  DynamicJsonDocument doc(4096);

  SECTION("fixmap") {
    SECTION("empty") {
      const char* input = "\x80";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 0);
    }

    SECTION("two integers") {
      const char* input = "\x82\xA3one\x01\xA3two\x02";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["one"] == 1);
      REQUIRE(obj["two"] == 2);
    }

    SECTION("key is str 8") {
      const char* input = "\x82\xd9\x03one\x01\xd9\x03two\x02";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["one"] == 1);
      REQUIRE(obj["two"] == 2);
    }

    SECTION("key is str 16") {
      const char* input = "\x82\xda\x00\x03one\x01\xda\x00\x03two\x02";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["one"] == 1);
      REQUIRE(obj["two"] == 2);
    }

    SECTION("key is str 32") {
      const char* input =
          "\x82\xdb\x00\x00\x00\x03one\x01\xdb\x00\x00\x00\x03two\x02";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["one"] == 1);
      REQUIRE(obj["two"] == 2);
    }
  }

  SECTION("map 16") {
    SECTION("empty") {
      const char* input = "\xDE\x00\x00";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 0);
    }

    SECTION("two strings") {
      const char* input = "\xDE\x00\x02\xA1H\xA5hello\xA1W\xA5world";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["H"] == "hello");
      REQUIRE(obj["W"] == "world");
    }
  }

  SECTION("map 32") {
    SECTION("empty") {
      const char* input = "\xDF\x00\x00\x00\x00";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 0);
    }

    SECTION("two floats") {
      const char* input =
          "\xDF\x00\x00\x00\x02\xA4zero\xCA\x00\x00\x00\x00\xA2pi\xCA\x40\x48"
          "\xF5\xC3";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonObject obj = doc.as<JsonObject>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonObject>());
      REQUIRE(obj.size() == 2);
      REQUIRE(obj["zero"] == 0.0f);
      REQUIRE(obj["pi"] == 3.14f);
    }
  }
}
