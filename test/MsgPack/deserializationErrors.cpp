// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(const char* input, MsgPackError expected,
                  uint8_t nestingLimit = 10) {
  DynamicJsonDocument variant;

  MsgPackError error = deserializeMsgPack(variant, input, nestingLimit);

  REQUIRE(error == expected);
}

TEST_CASE("Errors returned by deserializeMsgPack()") {
  SECTION("unsupported") {
    check("\xc4", MsgPackError::NotSupported);  // bin 8
    check("\xc5", MsgPackError::NotSupported);  // bin 16
    check("\xc6", MsgPackError::NotSupported);  // bin 32
    check("\xc7", MsgPackError::NotSupported);  // ext 8
    check("\xc8", MsgPackError::NotSupported);  // ext 16
    check("\xc9", MsgPackError::NotSupported);  // ext 32
    check("\xd4", MsgPackError::NotSupported);  // fixext 1
    check("\xd5", MsgPackError::NotSupported);  // fixext 2
    check("\xd6", MsgPackError::NotSupported);  // fixext 4
    check("\xd7", MsgPackError::NotSupported);  // fixext 8
    check("\xd8", MsgPackError::NotSupported);  // fixext 16
  }

  SECTION("unsupported in array") {
    check("\x91\xc4", MsgPackError::NotSupported);
  }

  SECTION("unsupported in map") {
    check("\x81\xc4\x00\xA1H", MsgPackError::NotSupported);
    check("\x81\xA1H\xc4\x00", MsgPackError::NotSupported);
  }

  SECTION("integer as key") {
    check("\x81\x01\xA1H", MsgPackError::NotSupported);
  }

  SECTION("object too deep") {
    check("\x80", MsgPackError::TooDeep, 0);           // {}
    check("\x80", MsgPackError::Ok, 1);                // {}
    check("\x81\xA1H\x80", MsgPackError::TooDeep, 1);  // {H:{}}
    check("\x81\xA1H\x80", MsgPackError::Ok, 2);       // {H:{}}
  }

  SECTION("array too deep") {
    check("\x90", MsgPackError::TooDeep, 0);      // []
    check("\x90", MsgPackError::Ok, 1);           // []
    check("\x91\x90", MsgPackError::TooDeep, 1);  // [[]]
    check("\x91\x90", MsgPackError::Ok, 2);       // [[]]
  }
}
