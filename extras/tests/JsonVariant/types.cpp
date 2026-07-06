// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>
#include <stdint.h>
#include <limits>

#include "Allocators.hpp"
#include "Literals.hpp"

template <typename T>
void checkReference(T& expected) {
  JsonVariant variant = expected;
  REQUIRE(expected == variant.as<T&>());
}

template <typename T>
void checkNumericType() {
  JsonDocument docMin, docMax;
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
  SpyingAllocator spy;
  JsonDocument doc(&spy);
  JsonVariant variant = doc.to<JsonVariant>();

#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("SizeOfJsonInteger") {
    REQUIRE(8 == sizeof(JsonInteger));
  }
#endif

  // /!\ Most test were moved to `JsonVariant/set.cpp`
  // TODO: move the remaining tests too

  SUBCASE("False") {
    variant.set(false);
    REQUIRE(variant.as<bool>() == false);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SUBCASE("True") {
    variant.set(true);
    REQUIRE(variant.as<bool>() == true);
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SUBCASE("Double") {
    checkNumericType<double>();
  }
  SUBCASE("Float") {
    checkNumericType<float>();
  }
  SUBCASE("SChar") {
    checkNumericType<signed char>();
  }
  SUBCASE("SInt") {
    checkNumericType<signed int>();
  }
  SUBCASE("SLong") {
    checkNumericType<signed long>();
  }
  SUBCASE("SShort") {
    checkNumericType<signed short>();
  }
  SUBCASE("UChar") {
    checkNumericType<unsigned char>();
  }
  SUBCASE("UInt") {
    checkNumericType<unsigned int>();
  }
  SUBCASE("ULong") {
    checkNumericType<unsigned long>();
  }
  SUBCASE("UShort") {
    checkNumericType<unsigned short>();
  }
#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("LongLong") {
    checkNumericType<unsigned long long>();
  }
  SUBCASE("ULongLong") {
    checkNumericType<unsigned long long>();
  }
#endif

  SUBCASE("Int8") {
    checkNumericType<int8_t>();
  }
  SUBCASE("Uint8") {
    checkNumericType<uint8_t>();
  }
  SUBCASE("Int16") {
    checkNumericType<int16_t>();
  }
  SUBCASE("Uint16") {
    checkNumericType<uint16_t>();
  }
  SUBCASE("Int32") {
    checkNumericType<int32_t>();
  }
  SUBCASE("Uint32") {
    checkNumericType<uint32_t>();
  }
#if ARDUINOJSON_USE_LONG_LONG
  SUBCASE("Int64") {
    checkNumericType<int64_t>();
  }
  SUBCASE("Uint64") {
    checkNumericType<uint64_t>();
  }
#endif

  SUBCASE("CanStoreObject") {
    JsonDocument doc2;
    JsonObject object = doc2.to<JsonObject>();

    variant.set(object);
    REQUIRE(variant.is<JsonObject>());
    REQUIRE(variant.as<JsonObject>() == object);
  }
}

TEST_CASE("volatile") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SUBCASE("volatile bool") {  // issue #2029
    volatile bool f = true;
    variant.set(f);
    CHECK(variant.is<bool>() == true);
    CHECK(variant.as<bool>() == true);
  }

  SUBCASE("volatile int") {
    volatile int f = 42;
    variant.set(f);
    CHECK(variant.is<int>() == true);
    CHECK(variant.as<int>() == 42);
  }

  SUBCASE("volatile float") {  // issue #1557
    volatile float f = 3.14f;
    variant.set(f);
    CHECK(variant.is<float>() == true);
    CHECK(variant.as<float>() == 3.14f);
  }

  SUBCASE("volatile double") {
    volatile double f = 3.14;
    variant.set(f);
    CHECK(variant.is<double>() == true);
    CHECK(variant.as<double>() == 3.14);
  }
}
