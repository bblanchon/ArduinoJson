// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <sstream>

using namespace ArduinoJson::TypeTraits;

TEST_CASE("TypeTraits") {
  SECTION("IsBaseOf") {
    REQUIRE_FALSE(
        static_cast<bool>(IsBaseOf<std::istream, std::ostringstream>::value));
    REQUIRE(
        static_cast<bool>(IsBaseOf<std::istream, std::istringstream>::value));
    REQUIRE(static_cast<bool>(
        IsBaseOf<JsonVariantBase<JsonObjectSubscript<const char*> >,
                 JsonObjectSubscript<const char*> >::value));
  }

  SECTION("IsArray") {
    REQUIRE_FALSE((IsArray<const char*>::value));
    REQUIRE((IsArray<const char[]>::value));
    REQUIRE((IsArray<const char[10]>::value));
  }

  SECTION("IsVariant") {
    REQUIRE(
        static_cast<bool>(IsVariant<JsonObjectSubscript<const char*> >::value));
    REQUIRE(static_cast<bool>(IsVariant<JsonVariant>::value));
  }

  SECTION("IsString") {
    REQUIRE((IsString<const char*>::value));
    REQUIRE((IsString<std::string>::value));
    REQUIRE_FALSE((IsString<double>::value));
  }
}
