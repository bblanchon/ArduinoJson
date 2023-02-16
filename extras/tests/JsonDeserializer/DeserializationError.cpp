// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include <sstream>

void testStringification(DeserializationError error, std::string expected) {
  REQUIRE(error.c_str() == expected);
}

void testBoolification(DeserializationError error, bool expected) {
  // DeserializationError on left-hand side
  CHECK(bool(error) == expected);
  CHECK(bool(error) != !expected);
  CHECK(!bool(error) == !expected);

  // DeserializationError on right-hand side
  CHECK(expected == bool(error));
  CHECK(!expected != bool(error));
  CHECK(!expected == !bool(error));
}

#define TEST_STRINGIFICATION(symbol) \
  testStringification(DeserializationError::symbol, #symbol)

#define TEST_BOOLIFICATION(symbol, expected) \
  testBoolification(DeserializationError::symbol, expected)

TEST_CASE("DeserializationError") {
  SECTION("c_str()") {
    TEST_STRINGIFICATION(Ok);
    TEST_STRINGIFICATION(EmptyInput);
    TEST_STRINGIFICATION(IncompleteInput);
    TEST_STRINGIFICATION(InvalidInput);
    TEST_STRINGIFICATION(NoMemory);
    TEST_STRINGIFICATION(TooDeep);
  }

  SECTION("as boolean") {
    TEST_BOOLIFICATION(Ok, false);
    TEST_BOOLIFICATION(EmptyInput, true);
    TEST_BOOLIFICATION(IncompleteInput, true);
    TEST_BOOLIFICATION(InvalidInput, true);
    TEST_BOOLIFICATION(NoMemory, true);
    TEST_BOOLIFICATION(TooDeep, true);
  }

  SECTION("ostream DeserializationError") {
    std::stringstream s;
    s << DeserializationError(DeserializationError::InvalidInput);
    REQUIRE(s.str() == "InvalidInput");
  }

  SECTION("ostream DeserializationError::Code") {
    std::stringstream s;
    s << DeserializationError::InvalidInput;
    REQUIRE(s.str() == "InvalidInput");
  }

  SECTION("switch") {
    DeserializationError err = DeserializationError::InvalidInput;
    switch (err.code()) {
      case DeserializationError::InvalidInput:
        SUCCEED();
        break;
      default:
        FAIL();
        break;
    }
  }

  SECTION("Use in a condition") {
    DeserializationError invalidInput(DeserializationError::InvalidInput);
    DeserializationError ok(DeserializationError::Ok);

    SECTION("if (!err)") {
      if (!invalidInput)
        FAIL();
    }

    SECTION("if (err)") {
      if (ok)
        FAIL();
    }
  }

  SECTION("Comparisons") {
    DeserializationError invalidInput(DeserializationError::InvalidInput);
    DeserializationError ok(DeserializationError::Ok);

    SECTION("DeserializationError == Code") {
      REQUIRE(invalidInput == DeserializationError::InvalidInput);
      REQUIRE(ok == DeserializationError::Ok);
    }

    SECTION("Code == DeserializationError") {
      REQUIRE(DeserializationError::InvalidInput == invalidInput);
      REQUIRE(DeserializationError::Ok == ok);
    }

    SECTION("DeserializationError != Code") {
      REQUIRE(invalidInput != DeserializationError::Ok);
      REQUIRE(ok != DeserializationError::InvalidInput);
    }

    SECTION("Code != DeserializationError") {
      REQUIRE(DeserializationError::Ok != invalidInput);
      REQUIRE(DeserializationError::InvalidInput != ok);
    }

    SECTION("DeserializationError == DeserializationError") {
      REQUIRE_FALSE(invalidInput == ok);
    }

    SECTION("DeserializationError != DeserializationError") {
      REQUIRE(invalidInput != ok);
    }
  }
}
