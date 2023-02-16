// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void checkMsgPackDocument(const char* input, size_t inputSize,
                                 const char* expectedJson) {
  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeMsgPack(doc, input, inputSize);

  REQUIRE(error == DeserializationError::Ok);
  std::string actualJson;
  serializeJson(doc, actualJson);
  REQUIRE(actualJson == expectedJson);
}

static void checkMsgPackError(const char* input, size_t inputSize,
                              DeserializationError expectedError) {
  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeMsgPack(doc, input, inputSize);

  REQUIRE(error == expectedError);
}

TEST_CASE("deserializeMsgPack() return NotSupported") {
  SECTION("bin 8") {
    checkMsgPackDocument("\x92\xc4\x01X\x2A", 5, "[null,42]");
  }

  SECTION("bin 16") {
    checkMsgPackDocument("\x92\xc5\x00\x01X\x2A", 6, "[null,42]");
  }

  SECTION("bin 32") {
    checkMsgPackDocument("\x92\xc6\x00\x00\x00\x01X\x2A", 8, "[null,42]");
  }

  SECTION("ext 8") {
    checkMsgPackDocument("\x92\xc7\x01\x01\x01\x2A", 6, "[null,42]");
  }

  SECTION("ext 16") {
    checkMsgPackDocument("\x92\xc8\x00\x01\x01\x01\x2A", 7, "[null,42]");
  }

  SECTION("ext 32") {
    checkMsgPackDocument("\x92\xc9\x00\x00\x00\x01\x01\x01\x2A", 9,
                         "[null,42]");
  }

  SECTION("fixext 1") {
    checkMsgPackDocument("\x92\xd4\x01\x01\x2A", 5, "[null,42]");
  }

  SECTION("fixext 2") {
    checkMsgPackDocument("\x92\xd5\x01\x01\x02\x2A", 6, "[null,42]");
  }

  SECTION("fixext 4") {
    checkMsgPackDocument("\x92\xd6\x01\x01\x02\x03\x04\x2A", 8, "[null,42]");
  }

  SECTION("fixext 8") {
    checkMsgPackDocument("\x92\xd7\x01\x01\x02\x03\x04\x05\x06\x07\x08\x2A", 12,
                         "[null,42]");
  }

  SECTION("fixext 16") {
    checkMsgPackDocument(
        "\x92\xd8\x01\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
        "\x0F\x10\x2A",
        20, "[null,42]");
  }

  SECTION("integer as key") {
    checkMsgPackError("\x81\x01\xA1H", 3, DeserializationError::InvalidInput);
  }
}
