// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void check(const char* input, DeserializationError expected,
                  uint8_t limit) {
  DynamicJsonDocument doc(4096);
  doc.nestingLimit = limit;

  DeserializationError error = deserializeMsgPack(doc, input);

  REQUIRE(error == expected);
}

TEST_CASE("Errors returned by deserializeMsgPack()") {
  SECTION("object too deep") {
    check("\x80", DeserializationError::TooDeep, 0);           // {}
    check("\x80", DeserializationError::Ok, 1);                // {}
    check("\x81\xA1H\x80", DeserializationError::TooDeep, 1);  // {H:{}}
    check("\x81\xA1H\x80", DeserializationError::Ok, 2);       // {H:{}}
  }

  SECTION("array too deep") {
    check("\x90", DeserializationError::TooDeep, 0);      // []
    check("\x90", DeserializationError::Ok, 1);           // []
    check("\x91\x90", DeserializationError::TooDeep, 1);  // [[]]
    check("\x91\x90", DeserializationError::Ok, 2);       // [[]]
  }
}
