// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_ENABLE_COMMENTS 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("Comments should produce InvalidInput") {
  DynamicJsonDocument doc(2048);

  const char* testCases[] = {
      "/*COMMENT*/  [\"hello\"]",
      "[/*COMMENT*/ \"hello\"]",
      "[\"hello\"/*COMMENT*/]",
      "[\"hello\"/*COMMENT*/,\"world\"]",
      "[\"hello\",/*COMMENT*/ \"world\"]",
      "[/*/\n]",
      "[/*COMMENT]",
      "[/*COMMENT*]",
      "//COMMENT\n\t[\"hello\"]",
      "[//COMMENT\n\"hello\"]",
      "[\"hello\"//COMMENT\r\n]",
      "[\"hello\"//COMMENT\n,\"world\"]",
      "[\"hello\",//COMMENT\n\"world\"]",
      "[/COMMENT\n]",
      "[//COMMENT",
      "/*COMMENT*/ {\"hello\":\"world\"}",
      "{/*COMMENT*/\"hello\":\"world\"}",
      "{\"hello\"/*COMMENT*/:\"world\"}",
      "{\"hello\":/*COMMENT*/\"world\"}",
      "{\"hello\":\"world\"/*COMMENT*/}",
      "//COMMENT\n {\"hello\":\"world\"}",
      "{//COMMENT\n\"hello\":\"world\"}",
      "{\"hello\"//COMMENT\n:\"world\"}",
      "{\"hello\"://COMMENT\n\"world\"}",
      "{\"hello\":\"world\"//COMMENT\n}",
      "/{\"hello\":\"world\"}",
      "{/\"hello\":\"world\"}",
      "{\"hello\"/:\"world\"}",
      "{\"hello\":/\"world\"}",
      "{\"hello\":\"world\"/}",
      "{\"hello\":\"world\"/,\"answer\":42}",
      "{\"hello\":\"world\",/\"answer\":42}",
  };
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::InvalidInput);
  }
}
