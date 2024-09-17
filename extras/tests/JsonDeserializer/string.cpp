// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
#include <catch.hpp>

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

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

  JsonDocument doc;

  for (size_t i = 0; i < testCount; i++) {
    const TestCase& testCase = testCases[i];
    CAPTURE(testCase.input);
    DeserializationError err = deserializeJson(doc, testCase.input);
    CHECK(err == DeserializationError::Ok);
    CHECK(doc.as<std::string>() == testCase.expectedOutput);
  }
}

TEST_CASE("\\u0000") {
  JsonDocument doc;

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

  JsonDocument doc;

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) ==
            DeserializationError::IncompleteInput);
  }
}

TEST_CASE("Escape single quote in single quoted string") {
  JsonDocument doc;

  DeserializationError err = deserializeJson(doc, "'ab\\\'cd'");
  REQUIRE(err == DeserializationError::Ok);
  CHECK(doc.as<std::string>() == "ab\'cd");
}

TEST_CASE("Escape double quote in double quoted string") {
  JsonDocument doc;

  DeserializationError err = deserializeJson(doc, "'ab\\\"cd'");
  REQUIRE(err == DeserializationError::Ok);
  CHECK(doc.as<std::string>() == "ab\"cd");
}

TEST_CASE("Invalid JSON string") {
  const char* testCases[] = {"'\\u'",     "'\\u000g'", "'\\u000'",
                             "'\\u000G'", "'\\u000/'", "'\\x1234'"};
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  JsonDocument doc;

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::InvalidInput);
  }
}

TEST_CASE("Allocation of the key fails") {
  TimebombAllocator timebomb(0);
  SpyingAllocator spy(&timebomb);
  JsonDocument doc(&spy);

  SECTION("Quoted string, first member") {
    REQUIRE(deserializeJson(doc, "{\"example\":1}") ==
            DeserializationError::NoMemory);
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofStringBuffer()),
                         });
  }

  SECTION("Quoted string, second member") {
    timebomb.setCountdown(3);
    REQUIRE(deserializeJson(doc, "{\"hello\":1,\"world\"}") ==
            DeserializationError::NoMemory);
    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("hello")),
                Allocate(sizeofPool()),
                AllocateFail(sizeofStringBuffer()),
                ReallocateFail(sizeofPool(), sizeofObject(1)),
            });
  }

  SECTION("Non-Quoted string, first member") {
    REQUIRE(deserializeJson(doc, "{example:1}") ==
            DeserializationError::NoMemory);
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofStringBuffer()),
                         });
  }

  SECTION("Non-Quoted string, second member") {
    timebomb.setCountdown(3);
    REQUIRE(deserializeJson(doc, "{hello:1,world}") ==
            DeserializationError::NoMemory);
    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofStringBuffer()),
                Reallocate(sizeofStringBuffer(), sizeofString("hello")),
                Allocate(sizeofPool()),
                AllocateFail(sizeofStringBuffer()),
                ReallocateFail(sizeofPool(), sizeofObject(1)),
            });
  }
}

TEST_CASE("String allocation fails") {
  SpyingAllocator spy(FailingAllocator::instance());
  JsonDocument doc(&spy);

  SECTION("Input is const char*") {
    REQUIRE(deserializeJson(doc, "\"hello\"") ==
            DeserializationError::NoMemory);
    REQUIRE(spy.log() == AllocatorLog{
                             AllocateFail(sizeofStringBuffer()),
                         });
  }
}

TEST_CASE("Deduplicate values") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  deserializeJson(doc, "[\"example\",\"example\"]");

  CHECK(doc[0].as<const char*>() == doc[1].as<const char*>());
  REQUIRE(spy.log() ==
          AllocatorLog{
              Allocate(sizeofPool()),
              Allocate(sizeofStringBuffer()),
              Reallocate(sizeofStringBuffer(), sizeofString("example")),
              Allocate(sizeofStringBuffer()),
              Deallocate(sizeofStringBuffer()),
              Reallocate(sizeofPool(), sizeofArray(2)),
          });
}

TEST_CASE("Deduplicate keys") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  deserializeJson(doc, "[{\"example\":1},{\"example\":2}]");

  const char* key1 = doc[0].as<JsonObject>().begin()->key().c_str();
  const char* key2 = doc[1].as<JsonObject>().begin()->key().c_str();
  CHECK(key1 == key2);

  REQUIRE(spy.log() ==
          AllocatorLog{
              Allocate(sizeofPool()),
              Allocate(sizeofStringBuffer()),
              Reallocate(sizeofStringBuffer(), sizeofString("example")),
              Allocate(sizeofStringBuffer()),
              Deallocate(sizeofStringBuffer()),
              Reallocate(sizeofPool(), sizeofArray(2) + 2 * sizeofObject(1)),
          });
}
