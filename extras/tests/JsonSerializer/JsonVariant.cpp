// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <limits>

#include "Literals.hpp"

template <typename T>
void check(T value, const std::string& expected) {
  JsonDocument doc;
  doc.to<JsonVariant>().set(value);
  char buffer[256] = "";
  size_t returnValue = serializeJson(doc, buffer, sizeof(buffer));
  REQUIRE(expected == buffer);
  REQUIRE(expected.size() == returnValue);
}

TEST_CASE("serializeJson(JsonVariant)") {
  SUBCASE("Undefined") {
    check(JsonVariant(), "null");
  }

  SUBCASE("Null string") {
    check(static_cast<char*>(0), "null");
  }

  SUBCASE("const char*") {
    check("hello", "\"hello\"");
  }

  SUBCASE("string") {
    check("hello"_s, "\"hello\"");

    SUBCASE("Escape quotation mark") {
      check("hello \"world\""_s, "\"hello \\\"world\\\"\"");
    }

    SUBCASE("Escape reverse solidus") {
      check("hello\\world"_s, "\"hello\\\\world\"");
    }

    SUBCASE("Don't escape solidus") {
      check("fifty/fifty"_s, "\"fifty/fifty\"");
    }

    SUBCASE("Don't escape single quote") {
      check("hello'world"_s, "\"hello'world\"");
    }

    SUBCASE("Escape backspace") {
      check("hello\bworld"_s, "\"hello\\bworld\"");
    }

    SUBCASE("Escape formfeed") {
      check("hello\fworld"_s, "\"hello\\fworld\"");
    }

    SUBCASE("Escape linefeed") {
      check("hello\nworld"_s, "\"hello\\nworld\"");
    }

    SUBCASE("Escape carriage return") {
      check("hello\rworld"_s, "\"hello\\rworld\"");
    }

    SUBCASE("Escape tab") {
      check("hello\tworld"_s, "\"hello\\tworld\"");
    }

    SUBCASE("NUL char") {
      check("hello\0world"_s, "\"hello\\u0000world\"");
    }
  }

  SUBCASE("SerializedValue<const char*>") {
    check(serialized("[1,2]"), "[1,2]");
  }

  SUBCASE("SerializedValue<std::string>") {
    check(serialized("[1,2]"_s), "[1,2]");
  }

  SUBCASE("Double") {
    check(3.1415927, "3.1415927");
  }

  SUBCASE("Float") {
    REQUIRE(sizeof(float) == 4);
    check(3.1415927f, "3.141593");
  }

  SUBCASE("Zero") {
    check(0, "0");
  }

  SUBCASE("Integer") {
    check(42, "42");
  }

  SUBCASE("NegativeLong") {
    check(-42, "-42");
  }

  SUBCASE("UnsignedLong") {
    check(4294967295UL, "4294967295");
  }

  SUBCASE("True") {
    check(true, "true");
  }

  SUBCASE("OneFalse") {
    check(false, "false");
  }

#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("NegativeInt64") {
    check(-9223372036854775807 - 1, "-9223372036854775808");
  }

  SUBCASE("PositiveInt64") {
    check(9223372036854775807, "9223372036854775807");
  }

  SUBCASE("UInt64") {
    check(18446744073709551615U, "18446744073709551615");
  }
#endif
}
