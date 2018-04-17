// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

void testStringification(JsonError error, std::string expected) {
  REQUIRE(error.c_str() == expected);
}

void testBoolification(JsonError error, bool expected) {
  CHECK(error == expected);
}

#define TEST_STRINGIFICATION(symbol) \
  testStringification(JsonError::symbol, #symbol)

#define TEST_BOOLIFICATION(symbol, expected) \
  testBoolification(JsonError::symbol, expected)

TEST_CASE("JsonError") {
  SECTION("c_str()") {
    TEST_STRINGIFICATION(Ok);
    TEST_STRINGIFICATION(TooDeep);
    TEST_STRINGIFICATION(NoMemory);
    TEST_STRINGIFICATION(InvalidInput);
  }

  SECTION("as boolean") {
    TEST_BOOLIFICATION(Ok, false);
    TEST_BOOLIFICATION(TooDeep, true);
    TEST_BOOLIFICATION(NoMemory, true);
    TEST_BOOLIFICATION(InvalidInput, true);
  }

  SECTION("ostream") {
    std::stringstream s;
    s << JsonError::InvalidInput;
    REQUIRE(s.str() == "InvalidInput");
  }
}
