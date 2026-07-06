// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <doctest.h>
#include <ArduinoJson/Numbers/arithmeticCompare.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("arithmeticCompare()") {
  SUBCASE("int vs uint8_t") {
    CHECK(arithmeticCompare<int, uint8_t>(256, 1) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<int, uint8_t>(41, 42) == COMPARE_RESULT_LESS);
    CHECK(arithmeticCompare<int, uint8_t>(42, 42) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<int, uint8_t>(43, 42) == COMPARE_RESULT_GREATER);
  }

  SUBCASE("unsigned vs int") {
    CHECK(arithmeticCompare<unsigned, int>(0, -1) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<unsigned, int>(42, 41) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<unsigned, int>(42, 42) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<unsigned, int>(42, 43) == COMPARE_RESULT_LESS);
  }

  SUBCASE("float vs int") {
    CHECK(arithmeticCompare<float, int>(42, 41) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<float, int>(42, 42) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<float, int>(42, 43) == COMPARE_RESULT_LESS);
  }

  SUBCASE("int vs unsigned") {
    CHECK(arithmeticCompare<int, unsigned>(-1, 0) == COMPARE_RESULT_LESS);
    CHECK(arithmeticCompare<int, unsigned>(0, 0) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<int, unsigned>(1, 0) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<int, unsigned>(42, 41) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<int, unsigned>(42, 42) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<int, unsigned>(42, 43) == COMPARE_RESULT_LESS);
  }

  SUBCASE("unsigned vs unsigned") {
    CHECK(arithmeticCompare<unsigned, unsigned>(42, 41) ==
          COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<unsigned, unsigned>(42, 42) ==
          COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<unsigned, unsigned>(42, 43) == COMPARE_RESULT_LESS);
  }

  SUBCASE("bool vs bool") {
    CHECK(arithmeticCompare<bool, bool>(false, false) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<bool, bool>(true, true) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<bool, bool>(false, true) == COMPARE_RESULT_LESS);
    CHECK(arithmeticCompare<bool, bool>(true, false) == COMPARE_RESULT_GREATER);
  }

  SUBCASE("bool vs int") {
    CHECK(arithmeticCompare<bool, int>(false, -1) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<bool, int>(false, 0) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<bool, int>(false, 1) == COMPARE_RESULT_LESS);
    CHECK(arithmeticCompare<bool, int>(true, 0) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<bool, int>(true, 1) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<bool, int>(true, 2) == COMPARE_RESULT_LESS);
  }

  SUBCASE("bool vs int") {
    CHECK(arithmeticCompare<int, bool>(0, false) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<int, bool>(1, true) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompare<int, bool>(1, false) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompare<int, bool>(0, true) == COMPARE_RESULT_LESS);
  }
}

TEST_CASE("arithmeticCompareNegateLeft()") {
  SUBCASE("unsigned vs int") {
    CHECK(arithmeticCompareNegateLeft<int>(0, 1) == COMPARE_RESULT_LESS);
    CHECK(arithmeticCompareNegateLeft<int>(42, -41) == COMPARE_RESULT_LESS);
    CHECK(arithmeticCompareNegateLeft<int>(42, -42) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompareNegateLeft<int>(42, -43) == COMPARE_RESULT_GREATER);
  }

  SUBCASE("unsigned vs unsigned") {
    CHECK(arithmeticCompareNegateLeft<unsigned>(42, 42) == COMPARE_RESULT_LESS);
  }
}

TEST_CASE("arithmeticCompareNegateRight()") {
  SUBCASE("int vs unsigned") {
    CHECK(arithmeticCompareNegateRight<int>(1, 0) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompareNegateRight<int>(-41, 42) == COMPARE_RESULT_GREATER);
    CHECK(arithmeticCompareNegateRight<int>(-42, 42) == COMPARE_RESULT_EQUAL);
    CHECK(arithmeticCompareNegateRight<int>(-43, 42) == COMPARE_RESULT_LESS);
  }

  SUBCASE("unsigned vs unsigned") {
    CHECK(arithmeticCompareNegateRight<unsigned>(42, 42) ==
          COMPARE_RESULT_GREATER);
  }
}
