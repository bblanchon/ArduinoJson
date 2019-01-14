// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

namespace my {
using ARDUINOJSON_NAMESPACE::isinf;
using ARDUINOJSON_NAMESPACE::isnan;
}  // namespace my

TEST_CASE("deserializeJson(DynamicJsonDocument&)") {
  DynamicJsonDocument doc(4096);

  SECTION("null char*") {
    DeserializationError err = deserializeJson(doc, static_cast<char*>(0));

    REQUIRE(err != DeserializationError::Ok);
  }

  SECTION("null const char*") {
    DeserializationError err =
        deserializeJson(doc, static_cast<const char*>(0));

    REQUIRE(err != DeserializationError::Ok);
  }

  SECTION("Integer") {
    DeserializationError err = deserializeJson(doc, "-42");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<int>());
    REQUIRE_FALSE(doc.is<bool>());
    REQUIRE(doc.as<int>() == -42);
  }

  SECTION("Double") {
    DeserializationError err = deserializeJson(doc, "-1.23e+4");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE_FALSE(doc.is<int>());
    REQUIRE(doc.is<double>());
    REQUIRE(doc.as<double>() == Approx(-1.23e+4));
  }

  SECTION("Double quoted string") {
    DeserializationError err = deserializeJson(doc, "\"hello world\"");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello world"));
  }

  SECTION("Single quoted string") {
    DeserializationError err = deserializeJson(doc, "\'hello world\'");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<char*>());
    REQUIRE_THAT(doc.as<char*>(), Equals("hello world"));
  }

  SECTION("Escape sequences") {
    DeserializationError err =
        deserializeJson(doc, "\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.as<std::string>() == "1\"2\\3/4\b5\f6\n7\r8\t9");
  }

  SECTION("UTF-16 surrogate") {
    DeserializationError err = deserializeJson(doc, "\"\\uD834\\uDD1E\"");

    REQUIRE(err == DeserializationError::NotSupported);
  }

  SECTION("True") {
    DeserializationError err = deserializeJson(doc, "true");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == true);
  }

  SECTION("False") {
    DeserializationError err = deserializeJson(doc, "false");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == false);
  }

  SECTION("0") {
    DeserializationError err = deserializeJson(doc, "0");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<int>() == true);
    REQUIRE(doc.as<int>() == 0);
    REQUIRE(doc.as<std::string>() == "0");  // issue #808
  }

  SECTION("NaN") {
    DeserializationError err = deserializeJson(doc, "NaN");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == true);
    REQUIRE(my::isnan(doc.as<float>()));
  }

  SECTION("Infinity") {
    DeserializationError err = deserializeJson(doc, "Infinity");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == true);
    REQUIRE(my::isinf(doc.as<float>()));
  }

  SECTION("+Infinity") {
    DeserializationError err = deserializeJson(doc, "+Infinity");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == true);
    REQUIRE(my::isinf(doc.as<float>()));
  }

  SECTION("-Infinity") {
    DeserializationError err = deserializeJson(doc, "-Infinity");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == true);
    REQUIRE(my::isinf(doc.as<float>()));
  }

  SECTION("issue #628") {
    DeserializationError err = deserializeJson(doc, "null");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == false);
  }

  SECTION("Should clear the JsonVariant") {
    deserializeJson(doc, "[1,2,3]");
    deserializeJson(doc, "{}");

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(doc.memoryUsage() == JSON_OBJECT_SIZE(0));
  }

  SECTION("Empty input") {
    DeserializationError err = deserializeJson(doc, "");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SECTION("Just a trailing comment") {
    DeserializationError err = deserializeJson(doc, "// comment");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SECTION("Just a block comment") {
    DeserializationError err = deserializeJson(doc, "/*comment*/");

    REQUIRE(err == DeserializationError::IncompleteInput);
  }

  SECTION("Just a slash") {
    DeserializationError err = deserializeJson(doc, "/");

    REQUIRE(err == DeserializationError::InvalidInput);
  }

  SECTION("Garbage") {
    DeserializationError err = deserializeJson(doc, "%*$£¤");

    REQUIRE(err == DeserializationError::InvalidInput);
  }

  SECTION("Premature null-terminator") {
    SECTION("In escape sequence") {
      DeserializationError err = deserializeJson(doc, "\"\\");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("In block comment") {
      DeserializationError err = deserializeJson(doc, "/* comment");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("In double quoted string") {
      DeserializationError err = deserializeJson(doc, "\"hello");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("In single quoted string") {
      DeserializationError err = deserializeJson(doc, "'hello");

      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SECTION("Premature end of input") {
    SECTION("In escape sequence") {
      DeserializationError err = deserializeJson(doc, "\"\\n\"", 2);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("In block comment") {
      DeserializationError err = deserializeJson(doc, "/* comment */", 10);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("In double quoted string") {
      DeserializationError err = deserializeJson(doc, "\"hello\"", 6);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }

    SECTION("In single quoted string") {
      DeserializationError err = deserializeJson(doc, "'hello'", 6);

      REQUIRE(err == DeserializationError::IncompleteInput);
    }
  }

  SECTION("Repeated object key") {
    DeserializationError err = deserializeJson(doc, "{a:{b:{c:1}},a:2}");

    REQUIRE(err == DeserializationError::Ok);
  }
}
