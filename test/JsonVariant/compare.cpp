// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename T>
void checkEquals(JsonVariant a, T b) {
  REQUIRE(b == a);
  REQUIRE(a == b);
  REQUIRE(b <= a);
  REQUIRE(a <= b);
  REQUIRE(b >= a);
  REQUIRE(a >= b);

  REQUIRE_FALSE(b != a);
  REQUIRE_FALSE(a != b);
  REQUIRE_FALSE(b > a);
  REQUIRE_FALSE(a > b);
  REQUIRE_FALSE(b < a);
  REQUIRE_FALSE(a < b);
}

template <typename T>
void checkGreater(JsonVariant a, T b) {
  REQUIRE(a > b);
  REQUIRE(b < a);
  REQUIRE(a != b);
  REQUIRE(b != a);

  REQUIRE_FALSE(a < b);
  REQUIRE_FALSE(b > a);
  REQUIRE_FALSE(a == b);
  REQUIRE_FALSE(b == a);
}

template <typename T>
void checkLower(JsonVariant a, T b) {
  REQUIRE(a < b);
  REQUIRE(b > a);
  REQUIRE(a != b);
  REQUIRE(b != a);

  REQUIRE_FALSE(a > b);
  REQUIRE_FALSE(b < a);
  REQUIRE_FALSE(a == b);
  REQUIRE_FALSE(b == a);
}

template <typename T>
void checkComparisons(T low, T mid, T high) {
  checkEquals(mid, mid);
  checkGreater(mid, low);
  checkLower(mid, high);
}

TEST_CASE("JsonVariant comparisons") {
  SECTION("Double") {
    checkComparisons<double>(123.44, 123.45, 123.46);
  }

  SECTION("Float") {
    checkComparisons<float>(123.44f, 123.45f, 123.46f);
  }

  SECTION("SChar") {
    checkComparisons<signed char>(122, 123, 124);
  }

  SECTION("SInt") {
    checkComparisons<signed int>(122, 123, 124);
  }

  SECTION("SLong") {
    checkComparisons<signed long>(122L, 123L, 124L);
  }

  SECTION("SShort") {
    checkComparisons<signed short>(122, 123, 124);
  }

  SECTION("UChar") {
    checkComparisons<unsigned char>(122, 123, 124);
  }

  SECTION("UInt") {
    checkComparisons<unsigned int>(122, 123, 124);
  }

  SECTION("ULong") {
    checkComparisons<unsigned long>(122L, 123L, 124L);
  }

  SECTION("UShort") {
    checkComparisons<unsigned short>(122, 123, 124);
  }

  SECTION("StringLiteral") {
    DynamicJsonDocument doc;
    deserializeJson(doc, "\"hello\"");
    JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE(variant == "hello");
    REQUIRE_FALSE(variant != "hello");

    REQUIRE(variant != "world");
    REQUIRE_FALSE(variant == "world");

    REQUIRE("hello" == variant);
    REQUIRE_FALSE("hello" != variant);

    REQUIRE("world" != variant);
    REQUIRE_FALSE("world" == variant);
  }

  SECTION("String") {
    JsonVariant variant = "hello";

    REQUIRE(variant == std::string("hello"));
    REQUIRE_FALSE(variant != std::string("hello"));

    REQUIRE(variant != std::string("world"));
    REQUIRE_FALSE(variant == std::string("world"));

    REQUIRE(std::string("hello") == variant);
    REQUIRE_FALSE(std::string("hello") != variant);

    REQUIRE(std::string("world") != variant);
    REQUIRE_FALSE(std::string("world") == variant);
  }

  SECTION("IntegerInVariant") {
    JsonVariant variant1 = 42;
    JsonVariant variant2 = 42;
    JsonVariant variant3 = 666;

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("StringInVariant") {
    JsonVariant variant1 = "0hello" + 1;  // make sure they have
    JsonVariant variant2 = "1hello" + 1;  // different addresses
    JsonVariant variant3 = "world";

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("DoubleInVariant") {
    JsonVariant variant1 = 42.0;
    JsonVariant variant2 = 42.0;
    JsonVariant variant3 = 666.0;

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("BoolInVariant") {
    JsonVariant variant1 = true;
    JsonVariant variant2 = true;
    JsonVariant variant3 = false;

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("ArrayInVariant") {
    DynamicJsonDocument doc1;
    JsonArray& array1 = doc1.to<JsonArray>();
    DynamicJsonDocument doc2;
    JsonArray& array2 = doc2.to<JsonArray>();

    JsonVariant variant1 = array1;
    JsonVariant variant2 = array1;
    JsonVariant variant3 = array2;

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("ObjectInVariant") {
    DynamicJsonDocument doc1;
    JsonObject& obj1 = doc1.to<JsonObject>();
    DynamicJsonDocument doc2;
    JsonObject& obj2 = doc2.to<JsonObject>();

    JsonVariant variant1 = obj1;
    JsonVariant variant2 = obj1;
    JsonVariant variant3 = obj2;

    REQUIRE(variant1 == variant2);
    REQUIRE_FALSE(variant1 != variant2);

    REQUIRE(variant1 != variant3);
    REQUIRE_FALSE(variant1 == variant3);
  }

  SECTION("VariantsOfDifferentTypes") {
    DynamicJsonDocument doc1;
    JsonObject& obj = doc1.to<JsonObject>();

    DynamicJsonDocument doc2;
    JsonArray& arr = doc2.to<JsonArray>();
    JsonVariant variants[] = {
        true, 42, 666.667, "hello", arr, obj,
    };
    size_t n = sizeof(variants) / sizeof(variants[0]);

    for (size_t i = 0; i < n; i++) {
      for (size_t j = i + 1; j < n; j++) {
        REQUIRE(variants[i] != variants[j]);
        REQUIRE_FALSE(variants[i] == variants[j]);
      }
    }
  }
}
