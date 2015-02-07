// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>

#include <ArduinoJson/Internals/QuotedString.hpp>
#include <ArduinoJson/Internals/StringBuilder.hpp>

using namespace ArduinoJson::Internals;

class QuotedString_PrintTo_Tests : public testing::Test {
 protected:
  void whenInputIs(const char *input) {
    StringBuilder sb(buffer, sizeof(buffer));
    returnValue = QuotedString::printTo(input, sb);
  }

  void outputMustBe(const char *expected) {
    EXPECT_STREQ(expected, buffer);
    EXPECT_EQ(strlen(expected), returnValue);
  }

 private:
  char buffer[1024];
  size_t returnValue;
};

TEST_F(QuotedString_PrintTo_Tests, Null) {
  whenInputIs(0);
  outputMustBe("null");
}

TEST_F(QuotedString_PrintTo_Tests, EmptyString) {
  whenInputIs("");
  outputMustBe("\"\"");
}

TEST_F(QuotedString_PrintTo_Tests, QuotationMark) {
  whenInputIs("\"");
  outputMustBe("\"\\\"\"");
}

TEST_F(QuotedString_PrintTo_Tests, ReverseSolidus) {
  whenInputIs("\\");
  outputMustBe("\"\\\\\"");
}

TEST_F(QuotedString_PrintTo_Tests, Solidus) {
  whenInputIs("/");
  outputMustBe("\"/\"");  // but the JSON format allows \/
}

TEST_F(QuotedString_PrintTo_Tests, Backspace) {
  whenInputIs("\b");
  outputMustBe("\"\\b\"");
}

TEST_F(QuotedString_PrintTo_Tests, Formfeed) {
  whenInputIs("\f");
  outputMustBe("\"\\f\"");
}

TEST_F(QuotedString_PrintTo_Tests, Newline) {
  whenInputIs("\n");
  outputMustBe("\"\\n\"");
}

TEST_F(QuotedString_PrintTo_Tests, CarriageReturn) {
  whenInputIs("\r");
  outputMustBe("\"\\r\"");
}

TEST_F(QuotedString_PrintTo_Tests, HorizontalTab) {
  whenInputIs("\t");
  outputMustBe("\"\\t\"");
}
