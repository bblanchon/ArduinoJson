// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <limits>

template <typename T>
void check(T value, const std::string &expected) {
  DynamicJsonDocument doc(4096);
  doc.to<JsonVariant>().set(value);
  char buffer[256] = "";
  size_t returnValue = serializeJson(doc, buffer, sizeof(buffer));
  REQUIRE(expected == buffer);
  REQUIRE(expected.size() == returnValue);
}

TEST_CASE("serializeJson(JsonVariant)") {
  SECTION("Undefined") {
    check(JsonVariant(), "null");
  }

  SECTION("Null string") {
    check(static_cast<char *>(0), "null");
  }

  SECTION("const char*") {
    check("hello", "\"hello\"");
  }

  SECTION("string") {
    check(std::string("hello"), "\"hello\"");

    SECTION("Escape quotation mark") {
      check(std::string("hello \"world\""), "\"hello \\\"world\\\"\"");
    }

    SECTION("Escape reverse solidus") {
      check(std::string("hello\\world"), "\"hello\\\\world\"");
    }

    SECTION("Don't escape solidus") {
      check(std::string("fifty/fifty"), "\"fifty/fifty\"");
    }

    SECTION("Escape backspace") {
      check(std::string("hello\bworld"), "\"hello\\bworld\"");
    }

    SECTION("Escape formfeed") {
      check(std::string("hello\fworld"), "\"hello\\fworld\"");
    }

    SECTION("Escape linefeed") {
      check(std::string("hello\nworld"), "\"hello\\nworld\"");
    }

    SECTION("Escape carriage return") {
      check(std::string("hello\rworld"), "\"hello\\rworld\"");
    }

    SECTION("Escape tab") {
      check(std::string("hello\tworld"), "\"hello\\tworld\"");
    }
  }

  SECTION("SerializedValue<const char*>") {
    check(serialized("[1,2]"), "[1,2]");
  }

  SECTION("SerializedValue<std::string>") {
    check(serialized(std::string("[1,2]")), "[1,2]");
  }

  SECTION("Double") {
    check(3.1415927, "3.1415927");
  }

  SECTION("Zero") {
    check(0, "0");
  }

  SECTION("Integer") {
    check(42, "42");
  }

  SECTION("NegativeLong") {
    check(-42, "-42");
  }

  SECTION("UnsignedLong") {
    check(4294967295UL, "4294967295");
  }

  SECTION("True") {
    check(true, "true");
  }

  SECTION("OneFalse") {
    check(false, "false");
  }

#if ARDUINOJSON_USE_LONG_LONG
  SECTION("NegativeInt64") {
    check(-9223372036854775807 - 1, "-9223372036854775808");
  }

  SECTION("PositiveInt64") {
    check(9223372036854775807, "9223372036854775807");
  }

  SECTION("UInt64") {
    check(18446744073709551615U, "18446744073709551615");
  }
#endif
}
