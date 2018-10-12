// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

TEST_CASE("Polyfills/type_traits") {
  SECTION("is_base_of") {
    REQUIRE_FALSE(
        static_cast<bool>(is_base_of<std::istream, std::ostringstream>::value));
    REQUIRE(
        static_cast<bool>(is_base_of<std::istream, std::istringstream>::value));
    REQUIRE(static_cast<bool>(
        is_base_of<JsonVariantBase<JsonObjectSubscript<const char*> >,
                   JsonObjectSubscript<const char*> >::value));
  }

  SECTION("is_array") {
    REQUIRE_FALSE((is_array<const char*>::value));
    REQUIRE((is_array<const char[]>::value));
    REQUIRE((is_array<const char[10]>::value));
  }

  SECTION("is_const") {
    CHECK(is_const<char>::value == false);
    CHECK(is_const<const char>::value == true);
  }

  SECTION("is_signed") {
    CHECK(is_signed<char>::value == true);
    CHECK(is_signed<signed char>::value == true);
    CHECK(is_signed<signed int>::value == true);
    CHECK(is_signed<signed short>::value == true);
    CHECK(is_signed<signed long>::value == true);
    CHECK(is_signed<float>::value == true);
    CHECK(is_signed<double>::value == true);
    CHECK(is_signed<bool>::value == false);
  }

  SECTION("is_unsigned") {
    CHECK(is_unsigned<unsigned char>::value == true);
    CHECK(is_unsigned<unsigned int>::value == true);
    CHECK(is_unsigned<unsigned short>::value == true);
    CHECK(is_unsigned<unsigned long>::value == true);
    CHECK(is_unsigned<bool>::value == true);
    CHECK(is_unsigned<char>::value == false);
    CHECK(is_unsigned<float>::value == false);
    CHECK(is_unsigned<double>::value == false);
  }

  SECTION("IsVisitable") {
    CHECK(IsVisitable<DeserializationError>::value == false);
    CHECK(IsVisitable<JsonPair>::value == false);
    CHECK(IsVisitable<JsonVariant>::value == true);
    CHECK(IsVisitable<JsonVariantConst>::value == true);
    CHECK(IsVisitable<JsonArray>::value == true);
    CHECK(IsVisitable<JsonArraySubscript>::value == true);
    CHECK(IsVisitable<JsonArrayConst>::value == true);
    CHECK(IsVisitable<JsonObject>::value == true);
    CHECK(IsVisitable<JsonObjectSubscript<const char*> >::value == true);
    CHECK(IsVisitable<JsonObjectConst>::value == true);
    CHECK(IsVisitable<DynamicJsonDocument>::value == true);
    CHECK(IsVisitable<StaticJsonDocument<10> >::value == true);
  }
}
