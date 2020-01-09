// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void checkNotSupported(const char* input) {
  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeMsgPack(doc, input);

  REQUIRE(error == DeserializationError::NotSupported);
}

TEST_CASE("deserializeMsgPack() return NotSupported") {
  SECTION("bin 8") {
    checkNotSupported("\xc4");
  }

  SECTION("bin 16") {
    checkNotSupported("\xc5");
  }

  SECTION("bin 32") {
    checkNotSupported("\xc6");
  }

  SECTION("ext 8") {
    checkNotSupported("\xc7");
  }

  SECTION("ext 16") {
    checkNotSupported("\xc8");
  }

  SECTION("ext 32") {
    checkNotSupported("\xc9");
  }

  SECTION("fixext 1") {
    checkNotSupported("\xd4");
  }

  SECTION("fixext 2") {
    checkNotSupported("\xd5");
  }

  SECTION("fixext 4") {
    checkNotSupported("\xd6");
  }

  SECTION("fixext 8") {
    checkNotSupported("\xd7");
  }

  SECTION("fixext 16") {
    checkNotSupported("\xd8");
  }

  SECTION("unsupported in array") {
    checkNotSupported("\x91\xc4");
  }

  SECTION("unsupported in map") {
    checkNotSupported("\x81\xc4\x00\xA1H");
    checkNotSupported("\x81\xA1H\xc4\x00");
  }

  SECTION("integer as key") {
    checkNotSupported("\x81\x01\xA1H");
  }
}
