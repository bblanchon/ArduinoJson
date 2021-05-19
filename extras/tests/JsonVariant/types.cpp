// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>
#include <limits>

template <typename T>
void checkValue(T expected) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  variant.set(expected);
  REQUIRE(expected == variant.as<T>());
}

template <typename T>
void checkReference(T &expected) {
  JsonVariant variant = expected;
  REQUIRE(expected == variant.as<T &>());
}

template <typename T>
void checkNumericType() {
  DynamicJsonDocument docMin(4096), docMax(4096);
  JsonVariant variantMin = docMin.to<JsonVariant>();
  JsonVariant variantMax = docMax.to<JsonVariant>();

  T min = std::numeric_limits<T>::min();
  T max = std::numeric_limits<T>::max();

  variantMin.set(min);
  variantMax.set(max);

  REQUIRE(min == variantMin.as<T>());
  REQUIRE(max == variantMax.as<T>());
}

TEST_CASE("JsonVariant set()/get()") {
#if ARDUINOJSON_USE_LONG_LONG
  SECTION("SizeOfJsonInteger") {
    REQUIRE(8 == sizeof(JsonInteger));
  }
#endif

  SECTION("Null") {
    checkValue<const char *>(NULL);
  }
  SECTION("const char*") {
    checkValue<const char *>("hello");
  }
  SECTION("std::string") {
    checkValue<std::string>("hello");
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
#if ARDUINOJSON_USE_LONG_LONG
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
#if ARDUINOJSON_USE_LONG_LONG
  SECTION("Int64") {
    checkNumericType<int64_t>();
  }
  SECTION("Uint64") {
    checkNumericType<uint64_t>();
  }
#endif

  SECTION("CanStoreObject") {
    DynamicJsonDocument doc(4096);
    JsonObject object = doc.to<JsonObject>();

    checkValue<JsonObject>(object);
  }
}

TEST_CASE("volatile") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("volatile int") {
    volatile int f = 42;
    variant.set(f);
    CHECK(variant.is<int>() == true);
    CHECK(variant.as<int>() == 42);
  }

  SECTION("volatile float") {  // issue #1557
    volatile float f = 3.14f;
    variant.set(f);
    CHECK(variant.is<float>() == true);
    CHECK(variant.as<float>() == 3.14f);
  }

  SECTION("volatile double") {
    volatile double f = 3.14;
    variant.set(f);
    CHECK(variant.is<double>() == true);
    CHECK(variant.as<double>() == 3.14);
  }
}
