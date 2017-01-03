// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace ArduinoJson::TypeTraits;

TEST(StdStream, IsBaseOf) {
  ASSERT_FALSE((IsBaseOf<std::istream, std::ostringstream>::value));
  ASSERT_TRUE((IsBaseOf<std::istream, std::istringstream>::value));
}
