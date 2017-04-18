// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

TEST_CASE("StringBuilder") {
  char output[20];
  StaticStringBuilder sb(output, sizeof(output));

  SECTION("InitialState") {
    REQUIRE(std::string("") == output);
  }

  SECTION("OverCapacity") {
    REQUIRE(19 == sb.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    REQUIRE(0 == sb.print("ABC"));
    REQUIRE(std::string("ABCDEFGHIJKLMNOPQRS") == output);
  }

  SECTION("EmptyString") {
    REQUIRE(0 == sb.print(""));
    REQUIRE(std::string("") == output);
  }

  SECTION("OneString") {
    REQUIRE(4 == sb.print("ABCD"));
    REQUIRE(std::string("ABCD") == output);
  }

  SECTION("TwoStrings") {
    REQUIRE(4 == sb.print("ABCD"));
    REQUIRE(4 == sb.print("EFGH"));
    REQUIRE(std::string("ABCDEFGH") == output);
  }
}
