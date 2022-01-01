// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Invalid JSON input") {
  const char* testCases[] = {"'\\u'",     "'\\u000g'", "'\\u000'", "'\\u000G'",
                             "'\\u000/'", "\\x1234",   "6a9",      "1,",
                             "2]",        "3}"};
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::InvalidInput);
  }
}

TEST_CASE("Invalid JSON input that should pass") {
  const char* testCases[] = {
      "nulL",
      "tru3",
      "fals3",
      "'\\ud83d'",         // leading surrogate without a trailing surrogate
      "'\\udda4'",         // trailing surrogate without a leading surrogate
      "'\\ud83d\\ud83d'",  // two leading surrogates
  };
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::Ok);
  }
}
