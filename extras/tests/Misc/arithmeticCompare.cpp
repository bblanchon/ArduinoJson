// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Numbers/arithmeticCompare.hpp>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("arithmeticCompare()") {
  SECTION("int vs uint8_t") {
    CHECK((arithmeticCompare<int, uint8_t>(256, 1) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<int, uint8_t>(41, 42) == COMPARE_RESULT_LESS));
    CHECK((arithmeticCompare<int, uint8_t>(42, 42) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<int, uint8_t>(43, 42) == COMPARE_RESULT_GREATER));
  }

  SECTION("unsigned vs int") {
    CHECK((arithmeticCompare<unsigned, int>(0, -1) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<unsigned, int>(42, 41) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<unsigned, int>(42, 42) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<unsigned, int>(42, 43) == COMPARE_RESULT_LESS));
  }

  SECTION("float vs int") {
    CHECK((arithmeticCompare<float, int>(42, 41) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<float, int>(42, 42) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<float, int>(42, 43) == COMPARE_RESULT_LESS));
  }

  SECTION("int vs unsigned") {
    CHECK((arithmeticCompare<int, unsigned>(-1, 0) == COMPARE_RESULT_LESS));
    CHECK((arithmeticCompare<int, unsigned>(0, 0) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<int, unsigned>(1, 0) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<int, unsigned>(42, 41) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<int, unsigned>(42, 42) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<int, unsigned>(42, 43) == COMPARE_RESULT_LESS));
  }

  SECTION("unsigned vs unsigned") {
    CHECK((arithmeticCompare<unsigned, unsigned>(42, 41) ==
           COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<unsigned, unsigned>(42, 42) ==
           COMPARE_RESULT_EQUAL));
    CHECK(
        (arithmeticCompare<unsigned, unsigned>(42, 43) == COMPARE_RESULT_LESS));
  }

  SECTION("bool vs bool") {
    CHECK(
        (arithmeticCompare<bool, bool>(false, false) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<bool, bool>(true, true) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<bool, bool>(false, true) == COMPARE_RESULT_LESS));
    CHECK(
        (arithmeticCompare<bool, bool>(true, false) == COMPARE_RESULT_GREATER));
  }

  SECTION("bool vs int") {
    CHECK((arithmeticCompare<bool, int>(false, -1) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<bool, int>(false, 0) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<bool, int>(false, 1) == COMPARE_RESULT_LESS));
    CHECK((arithmeticCompare<bool, int>(true, 0) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<bool, int>(true, 1) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<bool, int>(true, 2) == COMPARE_RESULT_LESS));
  }

  SECTION("bool vs int") {
    CHECK((arithmeticCompare<int, bool>(0, false) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<int, bool>(1, true) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompare<int, bool>(1, false) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompare<int, bool>(0, true) == COMPARE_RESULT_LESS));
  }
}

TEST_CASE("arithmeticCompareNegateLeft()") {
  SECTION("unsigned vs int") {
    CHECK((arithmeticCompareNegateLeft<int>(0, 1) == COMPARE_RESULT_LESS));
    CHECK((arithmeticCompareNegateLeft<int>(42, -41) == COMPARE_RESULT_LESS));
    CHECK((arithmeticCompareNegateLeft<int>(42, -42) == COMPARE_RESULT_EQUAL));
    CHECK(
        (arithmeticCompareNegateLeft<int>(42, -43) == COMPARE_RESULT_GREATER));
  }

  SECTION("unsigned vs unsigned") {
    CHECK(
        (arithmeticCompareNegateLeft<unsigned>(42, 42) == COMPARE_RESULT_LESS));
  }
}

TEST_CASE("arithmeticCompareNegateRight()") {
  SECTION("int vs unsigned") {
    CHECK((arithmeticCompareNegateRight<int>(1, 0) == COMPARE_RESULT_GREATER));
    CHECK(
        (arithmeticCompareNegateRight<int>(-41, 42) == COMPARE_RESULT_GREATER));
    CHECK((arithmeticCompareNegateRight<int>(-42, 42) == COMPARE_RESULT_EQUAL));
    CHECK((arithmeticCompareNegateRight<int>(-43, 42) == COMPARE_RESULT_LESS));
  }

  SECTION("unsigned vs unsigned") {
    CHECK((arithmeticCompareNegateRight<unsigned>(42, 42) ==
           COMPARE_RESULT_GREATER));
  }
}
