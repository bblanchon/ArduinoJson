// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

#include <sstream>

#include "Allocators.hpp"

TEST_CASE("deserializeMsgPack() returns InvalidInput") {
  JsonDocument doc;

  SUBCASE("integer as key") {
    auto err = deserializeMsgPack(doc, "\x81\x01\xA1H", 3);
    REQUIRE(err == DeserializationError::InvalidInput);
  }
}

TEST_CASE("deserializeMsgPack() returns EmptyInput") {
  JsonDocument doc;

  SUBCASE("from sized buffer") {
    auto err = deserializeMsgPack(doc, "", 0);

    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SUBCASE("from stream") {
    std::istringstream input("");

    auto err = deserializeMsgPack(doc, input);

    REQUIRE(err == DeserializationError::EmptyInput);
  }
}

static void testIncompleteInput(const char* input, size_t len) {
  JsonDocument doc;
  REQUIRE(deserializeMsgPack(doc, input, len) == DeserializationError::Ok);

  while (--len) {
    REQUIRE(deserializeMsgPack(doc, input, len) ==
            DeserializationError::IncompleteInput);
  }
}

TEST_CASE("deserializeMsgPack() returns IncompleteInput") {
  SUBCASE("empty input") {
    testIncompleteInput("\x00", 1);
  }

  SUBCASE("fixarray") {
    testIncompleteInput("\x91\x01", 2);
  }

  SUBCASE("array 16") {
    testIncompleteInput("\xDC\x00\x01\x01", 4);
  }

  SUBCASE("array 32") {
    testIncompleteInput("\xDD\x00\x00\x00\x01\x01", 6);
  }

  SUBCASE("fixmap") {
    testIncompleteInput("\x81\xA3one\x01", 6);
  }

  SUBCASE("map 16") {
    testIncompleteInput("\xDE\x00\x01\xA3one\x01", 8);
  }

  SUBCASE("map 32") {
    testIncompleteInput("\xDF\x00\x00\x00\x01\xA3one\x01", 10);
    testIncompleteInput("\xDF\x00\x00\x00\x01\xd9\x03one\x01", 11);
  }

  SUBCASE("uint 8") {
    testIncompleteInput("\xcc\x01", 2);
  }

  SUBCASE("uint 16") {
    testIncompleteInput("\xcd\x00\x01", 3);
  }

  SUBCASE("uint 32") {
    testIncompleteInput("\xCE\x00\x00\x00\x01", 5);
  }

#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("uint 64") {
    testIncompleteInput("\xCF\x00\x00\x00\x00\x00\x00\x00\x00", 9);
  }
#endif

  SUBCASE("int 8") {
    testIncompleteInput("\xD0\x01", 2);
  }

  SUBCASE("int 16") {
    testIncompleteInput("\xD1\x00\x01", 3);
  }

  SUBCASE("int 32") {
    testIncompleteInput("\xD2\x00\x00\x00\x01", 5);
  }

#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("int 64") {
    testIncompleteInput("\xD3\x00\x00\x00\x00\x00\x00\x00\x00", 9);
  }
#endif

  SUBCASE("float 32") {
    testIncompleteInput("\xCA\x40\x48\xF5\xC3", 5);
  }

  SUBCASE("float 64") {
    testIncompleteInput("\xCB\x40\x09\x21\xCA\xC0\x83\x12\x6F", 9);
  }

  SUBCASE("fixstr") {
    testIncompleteInput("\xABhello world", 12);
  }

  SUBCASE("str 8") {
    testIncompleteInput("\xd9\x05hello", 7);
  }

  SUBCASE("str 16") {
    testIncompleteInput("\xda\x00\x05hello", 8);
  }

  SUBCASE("str 32") {
    testIncompleteInput("\xdb\x00\x00\x00\x05hello", 10);
  }

  SUBCASE("bin 8") {
    testIncompleteInput("\xc4\x01X", 3);
  }

  SUBCASE("bin 16") {
    testIncompleteInput("\xc5\x00\x01X", 4);
  }

  SUBCASE("bin 32") {
    testIncompleteInput("\xc6\x00\x00\x00\x01X", 6);
  }

  SUBCASE("ext 8") {
    testIncompleteInput("\xc7\x01\x01\x01", 4);
  }

  SUBCASE("ext 16") {
    testIncompleteInput("\xc8\x00\x01\x01\x01", 5);
  }

  SUBCASE("ext 32") {
    testIncompleteInput("\xc9\x00\x00\x00\x01\x01\x01", 7);
  }

  SUBCASE("fixext 1") {
    testIncompleteInput("\xd4\x01\x01", 3);
  }

  SUBCASE("fixext 2") {
    testIncompleteInput("\xd5\x01\x01\x02", 4);
  }

  SUBCASE("fixext 4") {
    testIncompleteInput("\xd6\x01\x01\x02\x03\x04", 6);
  }

  SUBCASE("fixext 8") {
    testIncompleteInput("\xd7\x01\x01\x02\x03\x04\x05\x06\x07\x08", 10);
  }

  SUBCASE("fixext 16") {
    testIncompleteInput(
        "\xd8\x01\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E"
        "\x0F\x10",
        18);
  }
}

TEST_CASE(
    "deserializeMsgPack() returns NoMemory when string allocation fails") {
  TimebombAllocator allocator(0);
  JsonDocument doc(&allocator);

  SUBCASE("fixstr") {
    DeserializationError err = deserializeMsgPack(doc, "\xA5hello", 9);
    REQUIRE(err == DeserializationError::NoMemory);
  }

  SUBCASE("bin 8") {
    DeserializationError err = deserializeMsgPack(doc, "\xC4\x01X", 3);
    REQUIRE(err == DeserializationError::NoMemory);
  }
}

TEST_CASE(
    "deserializeMsgPack() returns NoMemory if extension allocation fails") {
  JsonDocument doc(FailingAllocator::instance());

  SUBCASE("uint32_t should pass") {
    auto err = deserializeMsgPack(doc, "\xceXXXX");

    REQUIRE(err == DeserializationError::Ok);
  }

  SUBCASE("uint64_t should fail") {
    auto err = deserializeMsgPack(doc, "\xcfXXXXXXXX");

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SUBCASE("int32_t should pass") {
    auto err = deserializeMsgPack(doc, "\xd2XXXX");

    REQUIRE(err == DeserializationError::Ok);
  }

  SUBCASE("int64_t should fail") {
    auto err = deserializeMsgPack(doc, "\xd3XXXXXXXX");

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SUBCASE("float should pass") {
    auto err = deserializeMsgPack(doc, "\xcaXXXX");

    REQUIRE(err == DeserializationError::Ok);
  }

  SUBCASE("double should fail") {
    auto err = deserializeMsgPack(doc, "\xcbXXXXXXXX");

    REQUIRE(err == DeserializationError::NoMemory);
  }
}
