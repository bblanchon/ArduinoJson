// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
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
      {"'\"'", "\""},
      {"'\\\\'", "\\"},
      {"'\\/'", "/"},
      {"'\\b'", "\b"},
      {"'\\f'", "\f"},
      {"'\\n'", "\n"},
      {"'\\r'", "\r"},
      {"'\\t'", "\t"},
      {"\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"", "1\"2\\3/4\b5\f6\n7\r8\t9"},
      {"'\\u0041'", "A"},
      {"'\\u00e4'", "\xc3\xa4"},                 // ä
      {"'\\u00E4'", "\xc3\xa4"},                 // ä
      {"'\\u3042'", "\xe3\x81\x82"},             // あ
      {"'\\ud83d\\udda4'", "\xf0\x9f\x96\xa4"},  // 🖤
      {"'\\uF053'", "\xef\x81\x93"},             // issue #1173
      {"'\\uF015'", "\xef\x80\x95"},             // issue #1173
      {"'\\uF054'", "\xef\x81\x94"},             // issue #1173
  };
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const TestCase& testCase = testCases[i];
    CAPTURE(testCase.input);
    DeserializationError err = deserializeJson(doc, testCase.input);
    CHECK(err == DeserializationError::Ok);
    CHECK(doc.as<std::string>() == testCase.expectedOutput);
  }
}

TEST_CASE("\\u0000") {
  StaticJsonDocument<200> doc;

  DeserializationError err = deserializeJson(doc, "\"wx\\u0000yz\"");
  REQUIRE(err == DeserializationError::Ok);

  const char* result = doc.as<const char*>();
  CHECK(result[0] == 'w');
  CHECK(result[1] == 'x');
  CHECK(result[2] == 0);
  CHECK(result[3] == 'y');
  CHECK(result[4] == 'z');
  CHECK(result[5] == 0);

  CHECK(doc.as<JsonString>().size() == 5);
  CHECK(doc.as<std::string>().size() == 5);
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
                             "'\\u000G'", "'\\u000/'", "'\\x1234'"};
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  DynamicJsonDocument doc(4096);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::InvalidInput);
  }
}

TEST_CASE("Not enough room to save the key") {
  DynamicJsonDocument doc(JSON_OBJECT_SIZE(1) + 8);

  SECTION("Quoted string") {
    REQUIRE(deserializeJson(doc, "{\"example\":1}") ==
            DeserializationError::Ok);
    REQUIRE(deserializeJson(doc, "{\"accuracy\":1}") ==
            DeserializationError::NoMemory);
    REQUIRE(deserializeJson(doc, "{\"hello\":1,\"world\"}") ==
            DeserializationError::NoMemory);  // fails in the second string
  }

  SECTION("Non-quoted string") {
    REQUIRE(deserializeJson(doc, "{example:1}") == DeserializationError::Ok);
    REQUIRE(deserializeJson(doc, "{accuracy:1}") ==
            DeserializationError::NoMemory);
    REQUIRE(deserializeJson(doc, "{hello:1,world}") ==
            DeserializationError::NoMemory);  // fails in the second string
  }
}

TEST_CASE("Empty memory pool") {
  // NOLINTNEXTLINE(clang-analyzer-optin.portability.UnixAPI)
  DynamicJsonDocument doc(0);

  SECTION("Input is const char*") {
    REQUIRE(deserializeJson(doc, "\"hello\"") ==
            DeserializationError::NoMemory);
    REQUIRE(deserializeJson(doc, "\"\"") == DeserializationError::NoMemory);
  }

  SECTION("Input is const char*") {
    char hello[] = "\"hello\"";
    REQUIRE(deserializeJson(doc, hello) == DeserializationError::Ok);
    char empty[] = "\"hello\"";
    REQUIRE(deserializeJson(doc, empty) == DeserializationError::Ok);
  }
}
