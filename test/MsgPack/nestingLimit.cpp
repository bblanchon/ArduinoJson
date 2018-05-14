// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(const char* input, MsgPackError expected, uint8_t limit) {
  DynamicJsonDocument doc;
  doc.nestingLimit = limit;

  MsgPackError error = deserializeMsgPack(doc, input);

  REQUIRE(error == expected);
}

TEST_CASE("Errors returned by deserializeMsgPack()") {
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
