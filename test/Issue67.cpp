// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include <gtest/gtest.h>
#include <ArduinoJson.h>

class Issue67 : public testing::Test {
 public:
  void whenInputIs(double value) { _variant = value; }

  void outputMustBe(const char* expected) {
    char buffer[1024];
    _variant.printTo(buffer, sizeof(buffer));
    ASSERT_STREQ(expected, buffer);
  }

 private:
  JsonVariant _variant;
};

TEST_F(Issue67, BigPositiveDouble) {
  whenInputIs(1e100);
  outputMustBe("1e+100");
}

TEST_F(Issue67, BigNegativeDouble) {
  whenInputIs(-1e100);
  outputMustBe("-1e+100");
}

TEST_F(Issue67, Zero) {
  whenInputIs(0.0);
  outputMustBe("0.00");
}

TEST_F(Issue67, SmallPositiveDouble) {
  whenInputIs(111.111);
  outputMustBe("111.11");
}

TEST_F(Issue67, SmallNegativeDouble) {
  whenInputIs(-111.111);
  outputMustBe("-111.11");
}
