// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

void testStringification(MsgPackError error, std::string expected) {
  REQUIRE(error.c_str() == expected);
}

void testBoolification(MsgPackError error, bool expected) {
  CHECK(error == expected);
}

#define TEST_STRINGIFICATION(symbol) \
  testStringification(MsgPackError::symbol, #symbol)

#define TEST_BOOLIFICATION(symbol, expected) \
  testBoolification(MsgPackError::symbol, expected)

TEST_CASE("MsgPackError") {
  SECTION("c_str()") {
    TEST_STRINGIFICATION(Ok);
    TEST_STRINGIFICATION(NotSupported);
    TEST_STRINGIFICATION(NoMemory);
    TEST_STRINGIFICATION(TooDeep);
    TEST_STRINGIFICATION(IncompleteInput);
  }

  SECTION("as boolean") {
    TEST_BOOLIFICATION(Ok, false);
    TEST_BOOLIFICATION(NotSupported, true);
    TEST_BOOLIFICATION(NoMemory, true);
    TEST_BOOLIFICATION(TooDeep, true);
    TEST_BOOLIFICATION(IncompleteInput, true);
  }

  SECTION("ostream") {
    std::stringstream s;
    s << MsgPackError::NotSupported;
    REQUIRE(s.str() == "NotSupported");
  }
}
