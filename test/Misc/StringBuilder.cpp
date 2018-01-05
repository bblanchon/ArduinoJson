// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::Internals;

template <typename StringBuilder, typename String>
void common_tests(StringBuilder& sb, const String& output) {
  SECTION("InitialState") {
    REQUIRE(std::string("") == output);
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

TEST_CASE("StaticStringBuilder") {
  char output[20];
  StaticStringBuilder sb(output, sizeof(output));

  common_tests(sb, static_cast<const char*>(output));

  SECTION("OverCapacity") {
    REQUIRE(19 == sb.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    REQUIRE(0 == sb.print("ABC"));
    REQUIRE(std::string("ABCDEFGHIJKLMNOPQRS") == output);
  }
}

TEST_CASE("DynamicStringBuilder") {
  std::string output;
  DynamicStringBuilder<std::string> sb(output);
  common_tests(sb, output);
}
