// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

DeserializationError deserialize(const char* input, size_t len) {
  DynamicJsonDocument doc(4096);

  return deserializeMsgPack(doc, input, len);
}

void checkAllSizes(const char* input, size_t len) {
  REQUIRE(deserialize(input, len) == DeserializationError::Ok);

  while (--len) {
    REQUIRE(deserialize(input, len) == DeserializationError::IncompleteInput);
  }
}

TEST_CASE("deserializeMsgPack() returns IncompleteInput") {
  SECTION("empty input") {
    checkAllSizes("\x00", 1);
  }

  SECTION("fixarray") {
    checkAllSizes("\x91\x01", 2);
  }

  SECTION("array 16") {
    checkAllSizes("\xDC\x00\x01\x01", 4);
  }

  SECTION("array 32") {
    checkAllSizes("\xDD\x00\x00\x00\x01\x01", 6);
  }

  SECTION("fixmap") {
    checkAllSizes("\x81\xA3one\x01", 6);
  }

  SECTION("map 16") {
    checkAllSizes("\xDE\x00\x01\xA3one\x01", 8);
  }

  SECTION("map 32") {
    checkAllSizes("\xDF\x00\x00\x00\x01\xA3one\x01", 10);
    checkAllSizes("\xDF\x00\x00\x00\x01\xd9\x03one\x01", 11);
  }

  SECTION("uint 8") {
    checkAllSizes("\xcc\x01", 2);
  }

  SECTION("uint 16") {
    checkAllSizes("\xcd\x00\x01", 3);
  }

  SECTION("uint 32") {
    checkAllSizes("\xCE\x00\x00\x00\x01", 5);
  }

#if ARDUINOJSON_USE_LONG_LONG
  SECTION("uint 64") {
    checkAllSizes("\xCF\x00\x00\x00\x00\x00\x00\x00\x00", 9);
  }
#endif

  SECTION("int 8") {
    checkAllSizes("\xD0\x01", 2);
  }

  SECTION("int 16") {
    checkAllSizes("\xD1\x00\x01", 3);
  }

  SECTION("int 32") {
    checkAllSizes("\xD2\x00\x00\x00\x01", 5);
  }

#if ARDUINOJSON_USE_LONG_LONG
  SECTION("int 64") {
    checkAllSizes("\xD3\x00\x00\x00\x00\x00\x00\x00\x00", 9);
  }
#endif

  SECTION("float 32") {
    checkAllSizes("\xCA\x40\x48\xF5\xC3", 5);
  }

  SECTION("float 64") {
    checkAllSizes("\xCB\x40\x09\x21\xCA\xC0\x83\x12\x6F", 9);
  }

  SECTION("fixstr") {
    checkAllSizes("\xABhello world", 12);
  }

  SECTION("str 8") {
    checkAllSizes("\xd9\x05hello", 7);
  }

  SECTION("str 16") {
    checkAllSizes("\xda\x00\x05hello", 8);
  }

  SECTION("str 32") {
    checkAllSizes("\xdb\x00\x00\x00\x05hello", 10);
  }

  SECTION("bin 8") {
    checkAllSizes("\xc4\x01X", 3);
  }

  SECTION("bin 16") {
    checkAllSizes("\xc5\x00\x01X", 4);
  }

  SECTION("bin 32") {
    checkAllSizes("\xc6\x00\x00\x00\x01X", 6);
  }

  SECTION("ext 8") {
    checkAllSizes("\xc7\x01\x01\x01", 4);
  }

  SECTION("ext 16") {
    checkAllSizes("\xc8\x00\x01\x01\x01", 5);
  }

  SECTION("ext 32") {
    checkAllSizes("\xc9\x00\x00\x00\x01\x01\x01", 7);
  }

  SECTION("fixext 1") {
    checkAllSizes("\xd4\x01\x01", 3);
  }

  SECTION("fixext 2") {
    checkAllSizes("\xd5\x01\x01\x02", 4);
  }

  SECTION("fixext 4") {
    checkAllSizes("\xd6\x01\x01\x02\x03\x04", 6);
  }

  SECTION("fixext 8") {
    checkAllSizes("\xd7\x01\x01\x02\x03\x04\x05\x06\x07\x08", 10);
  }

  SECTION("fixext 16") {
    checkAllSizes(
        "\xd8\x01\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
        "\x0F\x10",
        18);
  }
}
