// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Truncated JSON input") {
  const char* testCases[] = {"\"hello", "\'hello", "'\\u", "'\\u00", "'\\u000",
                             // false
                             "f", "fa", "fal", "fals",
                             // true
                             "t", "tr", "tru",
                             // null
                             "n", "nu", "nul",
                             // object
                             "{", "{a", "{a:", "{a:1", "{a:1,", "{a:1,"};
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) ==
            DeserializationError::IncompleteInput);
  }
}
