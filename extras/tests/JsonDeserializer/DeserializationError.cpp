// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

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
  SUBCASE("c_str()") {
    TEST_STRINGIFICATION(Ok);
    TEST_STRINGIFICATION(EmptyInput);
    TEST_STRINGIFICATION(IncompleteInput);
    TEST_STRINGIFICATION(InvalidInput);
    TEST_STRINGIFICATION(NoMemory);
    TEST_STRINGIFICATION(TooDeep);
  }

  SUBCASE("as boolean") {
    TEST_BOOLIFICATION(Ok, false);
    TEST_BOOLIFICATION(EmptyInput, true);
    TEST_BOOLIFICATION(IncompleteInput, true);
    TEST_BOOLIFICATION(InvalidInput, true);
    TEST_BOOLIFICATION(NoMemory, true);
    TEST_BOOLIFICATION(TooDeep, true);
  }

  SUBCASE("ostream DeserializationError") {
    std::stringstream s;
    s << DeserializationError(DeserializationError::InvalidInput);
    REQUIRE(s.str() == "InvalidInput");
  }

  SUBCASE("ostream DeserializationError::Code") {
    std::stringstream s;
    s << DeserializationError::InvalidInput;
    REQUIRE(s.str() == "InvalidInput");
  }

  SUBCASE("switch") {
    DeserializationError err = DeserializationError::InvalidInput;
    switch (err.code()) {
      case DeserializationError::InvalidInput:
        // SUCCEED();
        break;
      default:
        FAIL("Wrong case");
        break;
    }
  }

  SUBCASE("Use in a condition") {
    DeserializationError invalidInput(DeserializationError::InvalidInput);
    DeserializationError ok(DeserializationError::Ok);

    SUBCASE("if (!err)") {
      if (!invalidInput)
        FAIL("Error should evaluate to true");
    }

    SUBCASE("if (err)") {
      if (ok)
        FAIL("Ok should evaluate to false");
    }
  }

  SUBCASE("Comparisons") {
    DeserializationError invalidInput(DeserializationError::InvalidInput);
    DeserializationError ok(DeserializationError::Ok);

    SUBCASE("DeserializationError == Code") {
      REQUIRE(invalidInput == DeserializationError::InvalidInput);
      REQUIRE(ok == DeserializationError::Ok);
    }

    SUBCASE("Code == DeserializationError") {
      REQUIRE(DeserializationError::InvalidInput == invalidInput);
      REQUIRE(DeserializationError::Ok == ok);
    }

    SUBCASE("DeserializationError != Code") {
      REQUIRE(invalidInput != DeserializationError::Ok);
      REQUIRE(ok != DeserializationError::InvalidInput);
    }

    SUBCASE("Code != DeserializationError") {
      REQUIRE(DeserializationError::Ok != invalidInput);
      REQUIRE(DeserializationError::InvalidInput != ok);
    }

    SUBCASE("DeserializationError == DeserializationError") {
      REQUIRE_FALSE(invalidInput == ok);
    }

    SUBCASE("DeserializationError != DeserializationError") {
      REQUIRE(invalidInput != ok);
    }
  }
}
