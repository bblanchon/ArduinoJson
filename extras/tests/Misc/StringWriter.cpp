// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#define ARDUINOJSON_STRING_BUFFER_SIZE 5
#include <ArduinoJson.h>
#include <catch.hpp>
#include "custom_string.hpp"

using namespace ARDUINOJSON_NAMESPACE;

template <typename StringWriter>
static size_t print(StringWriter& writer, const char* s) {
  return writer.write(reinterpret_cast<const uint8_t*>(s), strlen(s));
}

template <typename StringWriter, typename String>
void common_tests(StringWriter& writer, const String& output) {
  SECTION("InitialState") {
    REQUIRE(std::string("") == output);
  }

  SECTION("EmptyString") {
    REQUIRE(0 == print(writer, ""));
    REQUIRE(std::string("") == output);
  }

  SECTION("OneString") {
    REQUIRE(4 == print(writer, "ABCD"));
    REQUIRE(std::string("ABCD") == output);
  }

  SECTION("TwoStrings") {
    REQUIRE(4 == print(writer, "ABCD"));
    REQUIRE(4 == print(writer, "EFGH"));
    REQUIRE(std::string("ABCDEFGH") == output);
  }
}

TEST_CASE("StaticStringWriter") {
  char output[20] = {0};
  StaticStringWriter writer(output, sizeof(output));

  common_tests(writer, static_cast<const char*>(output));

  SECTION("OverCapacity") {
    REQUIRE(20 == print(writer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    REQUIRE(0 == print(writer, "ABC"));
    REQUIRE("ABCDEFGHIJKLMNOPQRST" == std::string(output, 20));
  }
}

TEST_CASE("Writer<std::string>") {
  std::string output;
  Writer<std::string> writer(output);
  common_tests(writer, output);
}

TEST_CASE("Writer<String>") {
  ::String output;
  Writer< ::String> writer(output);

  SECTION("write(char)") {
    SECTION("writes to temporary buffer") {
      // accumulate in buffer
      writer.write('a');
      writer.write('b');
      writer.write('c');
      writer.write('d');
      REQUIRE(output == "");

      // flush when full
      writer.write('e');
      REQUIRE(output == "abcd");

      // flush on destruction
      writer.write('f');
      writer.~Writer();
      REQUIRE(output == "abcdef");
    }

    SECTION("returns 1 on success") {
      for (int i = 0; i < ARDUINOJSON_STRING_BUFFER_SIZE; i++) {
        REQUIRE(writer.write('x') == 1);
      }
    }

    SECTION("returns 0 on error") {
      output.limitCapacityTo(1);

      REQUIRE(writer.write('a') == 1);
      REQUIRE(writer.write('b') == 1);
      REQUIRE(writer.write('c') == 1);
      REQUIRE(writer.write('d') == 1);
      REQUIRE(writer.write('e') == 0);
      REQUIRE(writer.write('f') == 0);
    }
  }

  SECTION("write(char*, size_t)") {
    SECTION("empty string") {
      REQUIRE(0 == print(writer, ""));
      writer.flush();
      REQUIRE(output == "");
    }

    SECTION("writes to temporary buffer") {
      // accumulate in buffer
      print(writer, "abc");
      REQUIRE(output == "");

      // flush when full, and continue to accumulate
      print(writer, "de");
      REQUIRE(output == "abcd");

      // flush on destruction
      writer.~Writer();
      REQUIRE(output == "abcde");
    }
  }
}

TEST_CASE("Writer<custom_string>") {
  custom_string output;
  Writer<custom_string> writer(output);

  REQUIRE(4 == print(writer, "ABCD"));
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

TEST_CASE("serializeJson(doc, String)") {
  StaticJsonDocument<1024> doc;
  doc["hello"] = "world";
  ::String output;

  SECTION("sufficient capacity") {
    serializeJson(doc, output);
    REQUIRE(output == "{\"hello\":\"world\"}");
  }

  SECTION("unsufficient capacity") {  // issue #1561
    output.limitCapacityTo(10);
    serializeJson(doc, output);
    REQUIRE(output == "{\"hello\"");
  }
}
