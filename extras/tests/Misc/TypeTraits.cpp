// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
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

    CHECK(is_integral<UInt>::value == true);
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

  SECTION("is_convertible") {
    CHECK((is_convertible<short, int>::value == true));
    CHECK((is_convertible<int, int>::value == true));
    CHECK((is_convertible<EmptyEnum, int>::value == true));
    CHECK((is_convertible<int*, int>::value == false));
    CHECK((is_convertible<EmptyClass, int>::value == false));
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

  SECTION("IsVisitable") {
    CHECK(IsVisitable<DeserializationError>::value == false);
    CHECK(IsVisitable<JsonPair>::value == false);
    CHECK(IsVisitable<VariantRef>::value == true);
    CHECK(IsVisitable<VariantConstRef>::value == true);
    CHECK(IsVisitable<ArrayRef>::value == true);
    CHECK(IsVisitable<ElementProxy<ArrayRef> >::value == true);
    CHECK(IsVisitable<ArrayConstRef>::value == true);
    CHECK(IsVisitable<ObjectRef>::value == true);
    CHECK((IsVisitable<MemberProxy<ObjectRef, const char*> >::value == true));
    CHECK(IsVisitable<ObjectConstRef>::value == true);
    CHECK(IsVisitable<DynamicJsonDocument>::value == true);
    CHECK(IsVisitable<StaticJsonDocument<10> >::value == true);
  }
}
