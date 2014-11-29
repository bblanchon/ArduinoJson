// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson/Internals/StringBuilder.hpp>

using namespace ArduinoJson::Internals;

class StringBuilderTests : public testing::Test {
 protected:
  virtual void SetUp() { sb = new StringBuilder(buffer, sizeof(buffer)); }

  void print(const char *value) { returnValue = sb->print(value); }

  void outputMustBe(const char *expected) { EXPECT_STREQ(expected, buffer); }

  void resultMustBe(size_t expected) { EXPECT_EQ(expected, returnValue); }

 private:
  char buffer[20];
  Print *sb;
  size_t returnValue;
};

TEST_F(StringBuilderTests, InitialState) { outputMustBe(""); }

TEST_F(StringBuilderTests, OverCapacity) {
  print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  resultMustBe(19);

  print("ABC");
  resultMustBe(0);

  outputMustBe("ABCDEFGHIJKLMNOPQRS");
}

TEST_F(StringBuilderTests, EmptyString) {
  print("");
  resultMustBe(0);
  outputMustBe("");
}

TEST_F(StringBuilderTests, OneString) {
  print("ABCD");
  resultMustBe(4);
  outputMustBe("ABCD");
}

TEST_F(StringBuilderTests, TwoStrings) {
  print("ABCD");
  resultMustBe(4);

  print("EFGH");
  resultMustBe(4);

  outputMustBe("ABCDEFGH");
}
