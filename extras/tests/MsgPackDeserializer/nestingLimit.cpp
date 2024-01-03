// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <sstream>

#define SHOULD_WORK(expression) REQUIRE(DeserializationError::Ok == expression);
#define SHOULD_FAIL(expression) \
  REQUIRE(DeserializationError::TooDeep == expression);

TEST_CASE("JsonDeserializer nesting") {
  JsonDocument doc;

  SECTION("Input = const char*") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeMsgPack(doc, "\xA1H", nesting));  // "H"
      SHOULD_FAIL(deserializeMsgPack(doc, "\x90", nesting));   // []
      SHOULD_FAIL(deserializeMsgPack(doc, "\x80", nesting));   // {}
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeMsgPack(doc, "\x90", nesting));           // {}
      SHOULD_WORK(deserializeMsgPack(doc, "\x80", nesting));           // []
      SHOULD_FAIL(deserializeMsgPack(doc, "\x81\xA1H\x80", nesting));  // {H:{}}
      SHOULD_FAIL(deserializeMsgPack(doc, "\x91\x90", nesting));       // [[]]
    }
  }

  SECTION("char* and size_t") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeMsgPack(doc, "\xA1H", 2, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x90", 1, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x80", 1, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeMsgPack(doc, "\x90", 1, nesting));
      SHOULD_WORK(deserializeMsgPack(doc, "\x80", 1, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x81\xA1H\x80", 4, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x91\x90", 2, nesting));
    }
  }

  SECTION("Input = std::string") {
    using std::string;

    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeMsgPack(doc, string("\xA1H"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x90"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x80"), nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeMsgPack(doc, string("\x90"), nesting));
      SHOULD_WORK(deserializeMsgPack(doc, string("\x80"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x81\xA1H\x80"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x91\x90"), nesting));
    }
  }

  SECTION("Input = std::istream") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      std::istringstream good("\xA1H");  // "H"
      std::istringstream bad("\x90");    // []
      SHOULD_WORK(deserializeMsgPack(doc, good, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, bad, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      std::istringstream good("\x90");     // []
      std::istringstream bad("\x91\x90");  // [[]]
      SHOULD_WORK(deserializeMsgPack(doc, good, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, bad, nesting));
    }
  }
}
