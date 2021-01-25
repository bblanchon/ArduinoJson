// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_STRING_BUFFER_SIZE 5
#include <ArduinoJson.h>
#include <catch.hpp>
#include "custom_string.hpp"

using namespace ARDUINOJSON_NAMESPACE;

template <typename StringWriter>
static size_t print(StringWriter& sb, const char* s) {
  return sb.write(reinterpret_cast<const uint8_t*>(s), strlen(s));
}

template <typename StringWriter, typename String>
void common_tests(StringWriter& sb, const String& output) {
  SECTION("InitialState") {
    REQUIRE(std::string("") == output);
  }

  SECTION("EmptyString") {
    REQUIRE(0 == print(sb, ""));
    REQUIRE(std::string("") == output);
  }

  SECTION("OneString") {
    REQUIRE(4 == print(sb, "ABCD"));
    REQUIRE(std::string("ABCD") == output);
  }

  SECTION("TwoStrings") {
    REQUIRE(4 == print(sb, "ABCD"));
    REQUIRE(4 == print(sb, "EFGH"));
    REQUIRE(std::string("ABCDEFGH") == output);
  }
}

TEST_CASE("StaticStringWriter") {
  char output[20];
  StaticStringWriter sb(output, sizeof(output));

  common_tests(sb, static_cast<const char*>(output));

  SECTION("OverCapacity") {
    REQUIRE(19 == print(sb, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    REQUIRE(0 == print(sb, "ABC"));
    REQUIRE(std::string("ABCDEFGHIJKLMNOPQRS") == output);
  }
}

TEST_CASE("Writer<std::string>") {
  std::string output;
  Writer<std::string> sb(output);
  common_tests(sb, output);
}

TEST_CASE("Writer<String>") {
  ::String output;
  Writer< ::String> sb(output);

  common_tests(sb, output);

  SECTION("Writes characters to temporary buffer") {
    // accumulate in buffer
    sb.write('a');
    sb.write('b');
    sb.write('c');
    REQUIRE(output == "");

    // flush when full
    sb.write('d');
    REQUIRE(output == "abcd");

    // flush on destruction
    sb.write('e');
    sb.~Writer();
    REQUIRE(output == "abcde");
  }

  SECTION("Writes strings to temporary buffer") {
    // accumulate in buffer
    print(sb, "abc");
    REQUIRE(output == "");

    // flush when full, and continue to accumulate
    print(sb, "de");
    REQUIRE(output == "abcd");

    // flush on destruction
    sb.~Writer();
    REQUIRE(output == "abcde");
  }
}

TEST_CASE("Writer<custom_string>") {
  custom_string output;
  Writer<custom_string> sb(output);

  REQUIRE(4 == print(sb, "ABCD"));
  REQUIRE("ABCD" == output);
}

TEST_CASE("IsWriteableString") {
  SECTION("std::string") {
    REQUIRE(IsWriteableString<std::string>::value == true);
  }

  SECTION("custom_string") {
    REQUIRE(IsWriteableString<custom_string>::value == true);
  }

  SECTION("basic_string<wchar_t>") {
    REQUIRE(IsWriteableString<std::basic_string<wchar_t> >::value == false);
  }
}
