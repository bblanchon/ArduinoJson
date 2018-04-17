// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>
#include <limits>

template <typename T>
void checkValue(T expected) {
  JsonVariant variant = expected;
  REQUIRE(expected == variant.as<T>());
}

template <typename T>
void checkReference(T &expected) {
  JsonVariant variant = expected;
  REQUIRE(expected == variant.as<T &>());
}

template <typename T>
void checkNumericType() {
  T min = std::numeric_limits<T>::min();
  T max = std::numeric_limits<T>::max();

  JsonVariant variantMin(min);
  JsonVariant variantMax(max);

  REQUIRE(min == variantMin.as<T>());
  REQUIRE(max == variantMax.as<T>());
}

TEST_CASE("JsonVariant set()/get()") {
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("SizeOfJsonInteger") {
    REQUIRE(8 == sizeof(Internals::JsonInteger));
  }
#endif

  SECTION("Null") {
    checkValue<const char *>(NULL);
  }
  SECTION("String") {
    checkValue<const char *>("hello");
  }

  SECTION("False") {
    checkValue<bool>(false);
  }
  SECTION("True") {
    checkValue<bool>(true);
  }

  SECTION("Double") {
    checkNumericType<double>();
  }
  SECTION("Float") {
    checkNumericType<float>();
  }
  SECTION("Char") {
    checkNumericType<char>();
  }
  SECTION("SChar") {
    checkNumericType<signed char>();
  }
  SECTION("SInt") {
    checkNumericType<signed int>();
  }
  SECTION("SLong") {
    checkNumericType<signed long>();
  }
  SECTION("SShort") {
    checkNumericType<signed short>();
  }
  SECTION("UChar") {
    checkNumericType<unsigned char>();
  }
  SECTION("UInt") {
    checkNumericType<unsigned int>();
  }
  SECTION("ULong") {
    checkNumericType<unsigned long>();
  }
  SECTION("UShort") {
    checkNumericType<unsigned short>();
  }
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("LongLong") {
    checkNumericType<unsigned long long>();
  }
  SECTION("ULongLong") {
    checkNumericType<unsigned long long>();
  }
#endif

  SECTION("Int8") {
    checkNumericType<int8_t>();
  }
  SECTION("Uint8") {
    checkNumericType<uint8_t>();
  }
  SECTION("Int16") {
    checkNumericType<int16_t>();
  }
  SECTION("Uint16") {
    checkNumericType<uint16_t>();
  }
  SECTION("Int32") {
    checkNumericType<int32_t>();
  }
  SECTION("Uint32") {
    checkNumericType<uint32_t>();
  }
#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("Int64") {
    checkNumericType<int64_t>();
  }
  SECTION("Uint64") {
    checkNumericType<uint64_t>();
  }
#endif

  SECTION("CanStoreObject") {
    DynamicJsonDocument doc;
    JsonObject &object = doc.to<JsonObject>();

    checkReference<JsonObject>(object);
  }
}
