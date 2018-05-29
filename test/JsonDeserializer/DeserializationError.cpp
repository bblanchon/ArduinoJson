// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

void testStringification(DeserializationError error, std::string expected) {
  REQUIRE(error.c_str() == expected);
}

void testBoolification(DeserializationError error, bool expected) {
  CHECK(error == expected);
}

#define TEST_STRINGIFICATION(symbol) \
  testStringification(DeserializationError::symbol, #symbol)

#define TEST_BOOLIFICATION(symbol, expected) \
  testBoolification(DeserializationError::symbol, expected)

TEST_CASE("DeserializationError") {
  SECTION("c_str()") {
    TEST_STRINGIFICATION(Ok);
    TEST_STRINGIFICATION(TooDeep);
    TEST_STRINGIFICATION(NoMemory);
    TEST_STRINGIFICATION(InvalidInput);
    TEST_STRINGIFICATION(IncompleteInput);
    TEST_STRINGIFICATION(NotSupported);
  }

  SECTION("as boolean") {
    TEST_BOOLIFICATION(Ok, false);
    TEST_BOOLIFICATION(TooDeep, true);
    TEST_BOOLIFICATION(NoMemory, true);
    TEST_BOOLIFICATION(InvalidInput, true);
    TEST_BOOLIFICATION(IncompleteInput, true);
    TEST_BOOLIFICATION(NotSupported, true);
  }

  SECTION("ostream code") {
    std::stringstream s;
    s << DeserializationError(DeserializationError::InvalidInput);
    REQUIRE(s.str() == "InvalidInput");
  }

  SECTION("ostream code") {
    std::stringstream s;
    s << DeserializationError::InvalidInput;
    REQUIRE(s.str() == "InvalidInput");
  }
}
