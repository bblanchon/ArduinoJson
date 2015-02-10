// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/Internals/QuotedString.hpp>

using namespace ArduinoJson::Internals;

class QuotedString_ExtractFrom_Tests : public testing::Test {
 protected:
  void whenInputIs(const char *json) {
    strcpy(_jsonString, json);
    _result = QuotedString::extractFrom(_jsonString, &_trailing);
  }

  void resultMustBe(const char *expected) { EXPECT_STREQ(expected, _result); }

  void trailingMustBe(const char *expected) {
    EXPECT_STREQ(expected, _trailing);
  }

 private:
  char _jsonString[256];
  char *_result;
  char *_trailing;
};

TEST_F(QuotedString_ExtractFrom_Tests, EmptyDoubleQuotedString) {
  whenInputIs("\"\"");

  resultMustBe("");
  trailingMustBe("");
}

TEST_F(QuotedString_ExtractFrom_Tests, NoQuotes) {
  whenInputIs("hello world");

  resultMustBe(0);
}

TEST_F(QuotedString_ExtractFrom_Tests, MissingClosingQuote) {
  whenInputIs("\"hello world");

  resultMustBe(0);
}

TEST_F(QuotedString_ExtractFrom_Tests, EmptySingleQuotedString) {
  whenInputIs("''");

  resultMustBe("");
  trailingMustBe("");
}

TEST_F(QuotedString_ExtractFrom_Tests, SimpleDoubleQuotedString) {
  whenInputIs("\"hello world\"");

  resultMustBe("hello world");
  trailingMustBe("");
}

TEST_F(QuotedString_ExtractFrom_Tests, DoubleQuotedStringWithTrailing) {
  whenInputIs("\"hello\" world");

  resultMustBe("hello");
  trailingMustBe(" world");
}

TEST_F(QuotedString_ExtractFrom_Tests, SingleQuotedStringWithTrailing) {
  whenInputIs("'hello' world");

  resultMustBe("hello");
  trailingMustBe(" world");
}

TEST_F(QuotedString_ExtractFrom_Tests, CurlyBraces) {
  whenInputIs("\"{hello:world}\"");
  resultMustBe("{hello:world}");
}

TEST_F(QuotedString_ExtractFrom_Tests, SquareBraquets) {
  whenInputIs("\"[hello,world]\"");
  resultMustBe("[hello,world]");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedDoubleQuote) {
  whenInputIs("\"hello \\\"world\\\"\"");
  resultMustBe("hello \"world\"");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedSingleQuote) {
  whenInputIs("\"hello \\\'world\\\'\"");
  resultMustBe("hello 'world'");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedSolidus) {
  whenInputIs("\"hello \\/world\\/\"");
  resultMustBe("hello /world/");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedReverseSolidus) {
  whenInputIs("\"hello \\\\world\\\\\"");
  resultMustBe("hello \\world\\");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedBackspace) {
  whenInputIs("\"hello \\bworld\\b\"");
  resultMustBe("hello \bworld\b");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedFormfeed) {
  whenInputIs("\"hello \\fworld\\f\"");
  resultMustBe("hello \fworld\f");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedNewline) {
  whenInputIs("\"hello \\nworld\\n\"");
  resultMustBe("hello \nworld\n");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedCarriageReturn) {
  whenInputIs("\"hello \\rworld\\r\"");
  resultMustBe("hello \rworld\r");
}

TEST_F(QuotedString_ExtractFrom_Tests, EscapedTab) {
  whenInputIs("\"hello \\tworld\\t\"");
  resultMustBe("hello \tworld\t");
}

TEST_F(QuotedString_ExtractFrom_Tests, AllEscapedCharsTogether) {
  whenInputIs("\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"");
  resultMustBe("1\"2\\3/4\b5\f6\n7\r8\t9");
}
