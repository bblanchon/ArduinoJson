// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ARDUINOJSON_NAMESPACE;

class EmptyClass {};
enum EmptyEnum {};

TEST_CASE("Polyfills/type_traits") {
  SECTION("is_base_of") {
    REQUIRE_FALSE(
        static_cast<bool>(is_base_of<std::istream, std::ostringstream>::value));
    REQUIRE(
        static_cast<bool>(is_base_of<std::istream, std::istringstream>::value));
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

  SECTION("is_integral") {
    CHECK(is_integral<double>::value == false);
    CHECK(is_integral<float>::value == false);
    CHECK(is_integral<const double>::value == false);
    CHECK(is_integral<const float>::value == false);
    CHECK(is_integral<volatile double>::value == false);
    CHECK(is_integral<volatile float>::value == false);
    CHECK(is_integral<const volatile double>::value == false);
    CHECK(is_integral<const volatile float>::value == false);

    CHECK(is_integral<bool>::value == true);
    CHECK(is_integral<char>::value == true);
    CHECK(is_integral<signed char>::value == true);
    CHECK(is_integral<signed int>::value == true);
    CHECK(is_integral<signed long>::value == true);
    CHECK(is_integral<signed short>::value == true);
    CHECK(is_integral<unsigned char>::value == true);
    CHECK(is_integral<unsigned int>::value == true);
    CHECK(is_integral<unsigned long>::value == true);
    CHECK(is_integral<unsigned short>::value == true);
    CHECK(is_integral<const bool>::value == true);
    CHECK(is_integral<const char>::value == true);
    CHECK(is_integral<const signed char>::value == true);
    CHECK(is_integral<const signed int>::value == true);
    CHECK(is_integral<const signed long>::value == true);
    CHECK(is_integral<const signed short>::value == true);
    CHECK(is_integral<const unsigned char>::value == true);
    CHECK(is_integral<const unsigned int>::value == true);
    CHECK(is_integral<const unsigned long>::value == true);
    CHECK(is_integral<const unsigned short>::value == true);
    CHECK(is_integral<volatile bool>::value == true);
    CHECK(is_integral<volatile char>::value == true);
    CHECK(is_integral<volatile signed char>::value == true);
    CHECK(is_integral<volatile signed int>::value == true);
    CHECK(is_integral<volatile signed long>::value == true);
    CHECK(is_integral<volatile signed short>::value == true);
    CHECK(is_integral<volatile unsigned char>::value == true);
    CHECK(is_integral<volatile unsigned int>::value == true);
    CHECK(is_integral<volatile unsigned long>::value == true);
    CHECK(is_integral<volatile unsigned short>::value == true);
    CHECK(is_integral<const volatile bool>::value == true);
    CHECK(is_integral<const volatile char>::value == true);
    CHECK(is_integral<const volatile signed char>::value == true);
    CHECK(is_integral<const volatile signed int>::value == true);
    CHECK(is_integral<const volatile signed long>::value == true);
    CHECK(is_integral<const volatile signed short>::value == true);
    CHECK(is_integral<const volatile unsigned char>::value == true);
    CHECK(is_integral<const volatile unsigned int>::value == true);
    CHECK(is_integral<const volatile unsigned long>::value == true);
    CHECK(is_integral<const volatile unsigned short>::value == true);

    CHECK(is_integral<JsonUInt>::value == true);
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

    CHECK(is_signed<const char>::value == true);
    CHECK(is_signed<const signed char>::value == true);
    CHECK(is_signed<const signed int>::value == true);
    CHECK(is_signed<const signed short>::value == true);
    CHECK(is_signed<const signed long>::value == true);
    CHECK(is_signed<const float>::value == true);
    CHECK(is_signed<const double>::value == true);
    CHECK(is_signed<const bool>::value == false);

    CHECK(is_signed<volatile char>::value == true);
    CHECK(is_signed<volatile signed char>::value == true);
    CHECK(is_signed<volatile signed int>::value == true);
    CHECK(is_signed<volatile signed short>::value == true);
    CHECK(is_signed<volatile signed long>::value == true);
    CHECK(is_signed<volatile float>::value == true);
    CHECK(is_signed<volatile double>::value == true);
    CHECK(is_signed<volatile bool>::value == false);

    CHECK(is_signed<const volatile char>::value == true);
    CHECK(is_signed<const volatile signed char>::value == true);
    CHECK(is_signed<const volatile signed int>::value == true);
    CHECK(is_signed<const volatile signed short>::value == true);
    CHECK(is_signed<const volatile signed long>::value == true);
    CHECK(is_signed<const volatile float>::value == true);
    CHECK(is_signed<const volatile double>::value == true);
    CHECK(is_signed<const volatile bool>::value == false);
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

    CHECK(is_unsigned<const unsigned char>::value == true);
    CHECK(is_unsigned<const unsigned int>::value == true);
    CHECK(is_unsigned<const unsigned short>::value == true);
    CHECK(is_unsigned<const unsigned long>::value == true);
    CHECK(is_unsigned<const bool>::value == true);
    CHECK(is_unsigned<const char>::value == false);
    CHECK(is_unsigned<const float>::value == false);
    CHECK(is_unsigned<const double>::value == false);

    CHECK(is_unsigned<volatile unsigned char>::value == true);
    CHECK(is_unsigned<volatile unsigned int>::value == true);
    CHECK(is_unsigned<volatile unsigned short>::value == true);
    CHECK(is_unsigned<volatile unsigned long>::value == true);
    CHECK(is_unsigned<volatile bool>::value == true);
    CHECK(is_unsigned<volatile char>::value == false);
    CHECK(is_unsigned<volatile float>::value == false);
    CHECK(is_unsigned<volatile double>::value == false);

    CHECK(is_unsigned<const volatile unsigned char>::value == true);
    CHECK(is_unsigned<const volatile unsigned int>::value == true);
    CHECK(is_unsigned<const volatile unsigned short>::value == true);
    CHECK(is_unsigned<const volatile unsigned long>::value == true);
    CHECK(is_unsigned<const volatile bool>::value == true);
    CHECK(is_unsigned<const volatile char>::value == false);
    CHECK(is_unsigned<const volatile float>::value == false);
    CHECK(is_unsigned<const volatile double>::value == false);
  }

  SECTION("is_floating_point") {
    CHECK(is_floating_point<int>::value == false);
    CHECK(is_floating_point<float>::value == true);
    CHECK(is_floating_point<double>::value == true);
    CHECK(is_floating_point<const float>::value == true);
    CHECK(is_floating_point<const double>::value == true);
    CHECK(is_floating_point<volatile float>::value == true);
    CHECK(is_floating_point<volatile double>::value == true);
    CHECK(is_floating_point<const volatile float>::value == true);
    CHECK(is_floating_point<const volatile double>::value == true);
  }

  SECTION("is_convertible") {
    CHECK((is_convertible<short, int>::value == true));
    CHECK((is_convertible<int, int>::value == true));
    CHECK((is_convertible<EmptyEnum, int>::value == true));
    CHECK((is_convertible<int*, int>::value == false));
    CHECK((is_convertible<EmptyClass, int>::value == false));

    CHECK((is_convertible<DeserializationError, JsonVariantConst>::value ==
           false));
    CHECK((is_convertible<JsonPair, JsonVariantConst>::value == false));
    CHECK((is_convertible<JsonVariant, JsonVariantConst>::value == true));
    CHECK((is_convertible<JsonVariantConst, JsonVariantConst>::value == true));
    CHECK((is_convertible<JsonArray, JsonVariantConst>::value == true));
    CHECK((is_convertible<ElementProxy<JsonArray>, JsonVariantConst>::value ==
           true));
    CHECK((is_convertible<JsonArrayConst, JsonVariantConst>::value == true));
    CHECK((is_convertible<JsonObject, JsonVariantConst>::value == true));
    CHECK((is_convertible<MemberProxy<JsonObject, const char*>,
                          JsonVariantConst>::value == true));
    CHECK((is_convertible<JsonObjectConst, JsonVariantConst>::value == true));
    CHECK(
        (is_convertible<DynamicJsonDocument, JsonVariantConst>::value == true));
    CHECK((is_convertible<StaticJsonDocument<10>, JsonVariantConst>::value ==
           true));
  }

  SECTION("is_class") {
    CHECK((is_class<int>::value == false));
    CHECK((is_class<EmptyEnum>::value == false));
    CHECK((is_class<int*>::value == false));
    CHECK((is_class<EmptyClass>::value == true));
  }

  SECTION("is_enum") {
    CHECK(is_enum<int>::value == false);
    CHECK(is_enum<EmptyEnum>::value == true);
    CHECK(is_enum<int*>::value == false);
    CHECK(is_enum<EmptyClass>::value == false);
    CHECK(is_enum<bool>::value == false);
    CHECK(is_enum<double>::value == false);
  }
}
