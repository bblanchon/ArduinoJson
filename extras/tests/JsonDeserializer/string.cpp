// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("Valid JSON strings value") {
  struct TestCase {
    const char* input;
    const char* expectedOutput;
  };

  TestCase testCases[] = {
      {"\"hello world\"", "hello world"},
      {"\'hello world\'", "hello world"},
      {"\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"", "1\"2\\3/4\b5\f6\n7\r8\t9"},
      {"'\\u0041'", "A"},
      {"'\\u00e4'", "\xc3\xa4"},                 // ä
      {"'\\u00E4'", "\xc3\xa4"},                 // ä
      {"'\\u3042'", "\xe3\x81\x82"},             // あ
      {"'\\ud83d\\udda4'", "\xf0\x9f\x96\xa4"},  // 🖤
  };
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const TestCase& testCase = testCases[i];
    CAPTURE(testCase.input);
    DeserializationError err = deserializeJson(doc, testCase.input);
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == testCase.expectedOutput);
  }
}

TEST_CASE("Truncated JSON string") {
  const char* testCases[] = {"\"hello", "\'hello", "'\\u", "'\\u00", "'\\u000"};
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) ==
            DeserializationError::IncompleteInput);
  }
}

TEST_CASE("Invalid JSON string") {
  const char* testCases[] = {"'\\u'",     "'\\u000g'", "'\\u000'",
                             "'\\u000G'", "'\\u000/'", "\\x1234"};
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::InvalidInput);
  }
}

TEST_CASE("Not enough room to duplicate the string") {
  DynamicJsonDocument doc(4);

  REQUIRE(deserializeJson(doc, "\"hello world!\"") ==
          DeserializationError::NoMemory);
  REQUIRE(doc.isNull() == true);
}
