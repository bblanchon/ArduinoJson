// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

void testStringification(DeserializationError error, std::string expected) {
  REQUIRE(error.c_str() == expected);
}

void testBoolification(DeserializationError error, bool expected) {
  // DeserializationError on left-hand side
  CHECK(error == expected);
  CHECK(error != !expected);
  CHECK(!error == !expected);

  // DeserializationError on right-hand side
  CHECK(expected == error);
  CHECK(!expected != error);
  CHECK(!expected == !error);
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

  SECTION("out of range") {
    int code = 666;
    DeserializationError err(
        *reinterpret_cast<DeserializationError::Code*>(&code));
    REQUIRE(err.c_str() == std::string("???"));
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

  SECTION("Comparisons") {
    DeserializationError invalidInput(DeserializationError::InvalidInput);
    DeserializationError ok(DeserializationError::Ok);

    SECTION("DeserializationError == bool") {
      REQUIRE(invalidInput == true);
      REQUIRE(ok == false);
    }

    SECTION("bool == DeserializationError") {
      REQUIRE(true == invalidInput);
      REQUIRE(false == ok);
    }

    SECTION("DeserializationError != bool") {
      REQUIRE(invalidInput != false);
      REQUIRE(ok != true);
    }

    SECTION("bool != DeserializationError") {
      REQUIRE(false != invalidInput);
      REQUIRE(true != ok);
    }

    SECTION("Negations") {
      REQUIRE(!invalidInput == false);
      REQUIRE(!ok == true);
    }

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
