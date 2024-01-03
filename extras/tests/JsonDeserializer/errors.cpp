// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <catch.hpp>

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
