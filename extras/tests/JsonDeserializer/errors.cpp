// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

TEST_CASE("deserializeJson() returns IncompleteInput") {
  const char* testCases[] = {
      // strings
      "\"\\",
      "\"hello",
      "\'hello",
      // unicode
      "'\\u",
      "'\\u00",
      "'\\u000",
      // false
      "f",
      "fa",
      "fal",
      "fals",
      // true
      "t",
      "tr",
      "tru",
      // null
      "n",
      "nu",
      "nul",
      // object
      "{",
      "{a",
      "{a:",
      "{a:1",
      "{a:1,",
      "{a:1,b",
      "{a:1,b:",
  };

  for (auto input : testCases) {
    SECTION(input) {
      JsonDocument doc;
      REQUIRE(deserializeJson(doc, input) ==
              DeserializationError::IncompleteInput);
    }
  }
}

TEST_CASE("deserializeJson() returns InvalidInput") {
  const char* testCases[] = {
      // unicode
      "'\\u'", "'\\u000g'", "'\\u000'", "'\\u000G'", "'\\u000/'", "\\x1234",
      // numbers
      "6a9", "1,", "2]", "3}",
      // constants
      "nulL", "tru3", "fals3",
      // garbage
      "%*$£¤"};

  for (auto input : testCases) {
    SECTION(input) {
      JsonDocument doc;
      REQUIRE(deserializeJson(doc, input) ==
              DeserializationError::InvalidInput);
    }
  }
}

TEST_CASE("deserializeJson() oversees some edge cases") {
  const char* testCases[] = {
      "'\\ud83d'",         // leading surrogate without a trailing surrogate
      "'\\udda4'",         // trailing surrogate without a leading surrogate
      "'\\ud83d\\ud83d'",  // two leading surrogates
  };

  for (auto input : testCases) {
    SECTION(input) {
      JsonDocument doc;
      REQUIRE(deserializeJson(doc, input) == DeserializationError::Ok);
    }
  }
}

TEST_CASE("deserializeJson() returns EmptyInput") {
  JsonDocument doc;

  SECTION("null") {
    auto err = deserializeJson(doc, static_cast<const char*>(0));
    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SECTION("Empty string") {
    auto err = deserializeJson(doc, "");
    REQUIRE(err == DeserializationError::EmptyInput);
  }

  SECTION("Only spaces") {
    auto err = deserializeJson(doc, "  \t\n\r");
    REQUIRE(err == DeserializationError::EmptyInput);
  }
}

TEST_CASE("deserializeJson() returns NoMemory if string length overflows") {
  JsonDocument doc;
  auto maxLength = ArduinoJson::detail::StringNode::maxLength;

  SECTION("max length should succeed") {
    auto err = deserializeJson(doc, "\"" + std::string(maxLength, 'a') + "\"");

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("one above max length should fail") {
    auto err =
        deserializeJson(doc, "\"" + std::string(maxLength + 1, 'a') + "\"");
    REQUIRE(err == DeserializationError::NoMemory);
  }
}

TEST_CASE("deserializeJson() returns NoMemory if extension allocation fails") {
  JsonDocument doc(FailingAllocator::instance());

  SECTION("uint32_t should pass") {
    auto err = deserializeJson(doc, "4294967295");

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("uint64_t should fail") {
    auto err = deserializeJson(doc, "18446744073709551615");

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SECTION("int32_t should pass") {
    auto err = deserializeJson(doc, "-2147483648");

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("int64_t should fail") {
    auto err = deserializeJson(doc, "-9223372036854775808");

    REQUIRE(err == DeserializationError::NoMemory);
  }

  SECTION("float should pass") {
    auto err = deserializeJson(doc, "3.402823e38");

    REQUIRE(err == DeserializationError::Ok);
  }

  SECTION("double should fail") {
    auto err = deserializeJson(doc, "1.7976931348623157e308");

    REQUIRE(err == DeserializationError::NoMemory);
  }
}
