// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include "Allocators.hpp"

TEST_CASE("deserialize MsgPack array") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SUBCASE("fixarray") {
    SUBCASE("empty") {
      const char* input = "\x90";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 0);
    }

    SUBCASE("two integers") {
      const char* input = "\x92\x01\x02";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == 1);
      REQUIRE(array[1] == 2);
    }

    SUBCASE("tiny strings") {
      DeserializationError error =
          deserializeMsgPack(doc, "\x92\xA3xxx\xA3yyy");

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(doc.is<JsonArray>());
      REQUIRE(doc.size() == 2);
      REQUIRE(doc[0] == "xxx");
      REQUIRE(doc[1] == "yyy");
      REQUIRE(spy.log() == AllocatorLog{
                               Allocate(sizeofPool()),
                               Allocate(sizeofString("xxx")),
                               // Buffer is reused for the next string
                               Deallocate(sizeofString("xxx")),
                               Reallocate(sizeofPool(), sizeofPool(2)),
                           });
    }
  }

  SUBCASE("array 16") {
    SUBCASE("empty") {
      const char* input = "\xDC\x00\x00";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 0);
    }

    SUBCASE("two strings") {
      const char* input = "\xDC\x00\x02\xA5hello\xA5world";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 2);
      REQUIRE(array[0] == "hello");
      REQUIRE(array[1] == "world");
    }
  }

  SUBCASE("array 32") {
    SUBCASE("empty") {
      const char* input = "\xDD\x00\x00\x00\x00";

      DeserializationError error = deserializeMsgPack(doc, input);
      JsonArray array = doc.as<JsonArray>();

      REQUIRE(error == DeserializationError::Ok);
      REQUIRE(array.size() == 0);
    }

    SUBCASE("two floats") {
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
