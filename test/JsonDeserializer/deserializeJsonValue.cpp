// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("deserializeJson(DynamicJsonDocument&)") {
  DynamicJsonDocument doc;

  SECTION("null char*") {
    JsonError err = deserializeJson(doc, static_cast<char*>(0));

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("null const char*") {
    JsonError err = deserializeJson(doc, static_cast<const char*>(0));

    REQUIRE(err != JsonError::Ok);
  }

  SECTION("Integer") {
    JsonError err = deserializeJson(doc, "-42");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<int>());
    REQUIRE_FALSE(doc.is<bool>());
    REQUIRE(doc.as<int>() == -42);
  }

  SECTION("Double") {
    JsonError err = deserializeJson(doc, "-1.23e+4");

    REQUIRE(err == JsonError::Ok);
    REQUIRE_FALSE(doc.is<int>());
    REQUIRE(doc.is<double>());
    REQUIRE(doc.as<double>() == Approx(-1.23e+4));
  }

  SECTION("Double quoted string") {
    JsonError err = deserializeJson(doc, "\"hello world\"");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello world"));
  }

  SECTION("Single quoted string") {
    JsonError err = deserializeJson(doc, "\'hello world\'");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello world"));
  }

  SECTION("Escape sequences") {
    JsonError err =
        deserializeJson(doc, "\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.as<std::string>() == "1\"2\\3/4\b5\f6\n7\r8\t9");
  }

  SECTION("True") {
    JsonError err = deserializeJson(doc, "true");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == true);
  }

  SECTION("False") {
    JsonError err = deserializeJson(doc, "false");

    REQUIRE(err == JsonError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == false);
  }

  SECTION("Should clear the JsonVariant") {
    deserializeJson(doc, "[1,2,3]");
    deserializeJson(doc, "{}");

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
  }

  SECTION("Empty input") {
    JsonError err = deserializeJson(doc, "");

    REQUIRE(err == JsonError::IncompleteInput);
  }

  SECTION("Just a trailing comment") {
    JsonError err = deserializeJson(doc, "// comment");

    REQUIRE(err == JsonError::IncompleteInput);
  }

  SECTION("Just a block comment") {
    JsonError err = deserializeJson(doc, "/*comment*/");

    REQUIRE(err == JsonError::IncompleteInput);
  }

  SECTION("Just a slash") {
    JsonError err = deserializeJson(doc, "/");

    REQUIRE(err == JsonError::InvalidInput);
  }

  SECTION("Garbage") {
    JsonError err = deserializeJson(doc, "%*$£¤");

    REQUIRE(err == JsonError::InvalidInput);
  }

  SECTION("Premature null-terminator") {
    SECTION("In escape sequence") {
      JsonError err = deserializeJson(doc, "\"\\");

      REQUIRE(err == JsonError::IncompleteInput);
    }

    SECTION("In block comment") {
      JsonError err = deserializeJson(doc, "/* comment");

      REQUIRE(err == JsonError::IncompleteInput);
    }

    SECTION("In double quoted string") {
      JsonError err = deserializeJson(doc, "\"hello");

      REQUIRE(err == JsonError::IncompleteInput);
    }

    SECTION("In single quoted string") {
      JsonError err = deserializeJson(doc, "'hello");

      REQUIRE(err == JsonError::IncompleteInput);
    }
  }

  SECTION("Premature end of input") {
    SECTION("In escape sequence") {
      JsonError err = deserializeJson(doc, "\"\\n\"", 2);

      REQUIRE(err == JsonError::IncompleteInput);
    }

    SECTION("In block comment") {
      JsonError err = deserializeJson(doc, "/* comment */", 10);

      REQUIRE(err == JsonError::IncompleteInput);
    }

    SECTION("In double quoted string") {
      JsonError err = deserializeJson(doc, "\"hello\"", 6);

      REQUIRE(err == JsonError::IncompleteInput);
    }

    SECTION("In single quoted string") {
      JsonError err = deserializeJson(doc, "'hello'", 6);

      REQUIRE(err == JsonError::IncompleteInput);
    }
  }
}
