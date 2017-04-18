// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

static const char* null = 0;

TEST_CASE("JsonVariant::as()") {
  SECTION("DoubleAsBool") {
    JsonVariant variant = 4.2;
    REQUIRE(variant.as<bool>());
  }

  SECTION("DoubleAsCstr") {
    JsonVariant variant = 4.2;
    REQUIRE_FALSE(variant.as<const char*>());
  }

  SECTION("DoubleAsString") {
    JsonVariant variant = 4.2;
    REQUIRE(std::string("4.20") == variant.as<std::string>());
  }

  SECTION("DoubleAsLong") {
    JsonVariant variant = 4.2;
    REQUIRE(4L == variant.as<long>());
  }

  SECTION("DoubleAsUnsigned") {
    JsonVariant variant = 4.2;
    REQUIRE(4U == variant.as<unsigned>());
  }

  SECTION("DoubleZeroAsBool") {
    JsonVariant variant = 0.0;
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("DoubleZeroAsLong") {
    JsonVariant variant = 0.0;
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("FalseAsBool") {
    JsonVariant variant = false;
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("FalseAsDouble") {
    JsonVariant variant = false;
    REQUIRE(0.0 == variant.as<double>());
  }

  SECTION("FalseAsLong") {
    JsonVariant variant = false;
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("FalseAsString") {
    JsonVariant variant = false;
    REQUIRE(std::string("false") == variant.as<std::string>());
  }

  SECTION("TrueAsBool") {
    JsonVariant variant = true;
    REQUIRE(variant.as<bool>());
  }

  SECTION("TrueAsDouble") {
    JsonVariant variant = true;
    REQUIRE(1.0 == variant.as<double>());
  }

  SECTION("TrueAsLong") {
    JsonVariant variant = true;
    REQUIRE(1L == variant.as<long>());
  }

  SECTION("TrueAsString") {
    JsonVariant variant = true;
    REQUIRE(std::string("true") == variant.as<std::string>());
  }

  SECTION("LongAsBool") {
    JsonVariant variant = 42L;
    REQUIRE(variant.as<bool>());
  }

  SECTION("LongZeroAsBool") {
    JsonVariant variant = 0L;
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("PositiveLongAsDouble") {
    JsonVariant variant = 42L;
    REQUIRE(42.0 == variant.as<double>());
  }

  SECTION("NegativeLongAsDouble") {
    JsonVariant variant = -42L;
    REQUIRE(-42.0 == variant.as<double>());
  }

  SECTION("LongAsString") {
    JsonVariant variant = 42L;
    REQUIRE(std::string("42") == variant.as<std::string>());
  }

  SECTION("LongZeroAsDouble") {
    JsonVariant variant = 0L;
    REQUIRE(0.0 == variant.as<double>());
  }

  SECTION("NullAsBool") {
    JsonVariant variant = null;
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("NullAsDouble") {
    JsonVariant variant = null;
    REQUIRE(0.0 == variant.as<double>());
  }

  SECTION("NullAsLong") {
    JsonVariant variant = null;
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("NullAsString") {
    JsonVariant variant = null;
    REQUIRE(std::string("null") == variant.as<std::string>());
  }

  SECTION("NumberStringAsBool") {
    JsonVariant variant = "42";
    REQUIRE(variant.as<bool>());
  }

  SECTION("NumberStringAsLong") {
    JsonVariant variant = "42";
    REQUIRE(42L == variant.as<long>());
  }

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("NumberStringAsInt64Negative") {
    JsonVariant variant = "-9223372036854775808";
    REQUIRE(-9223372036854775807 - 1 == variant.as<long long>());
  }

  SECTION("NumberStringAsInt64Positive") {
    JsonVariant variant = "9223372036854775807";
    REQUIRE(9223372036854775807 == variant.as<long long>());
  }
#endif

  SECTION("RandomStringAsBool") {
    JsonVariant variant = "hello";
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("RandomStringAsLong") {
    JsonVariant variant = "hello";
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("RandomStringAsConstCharPtr") {
    JsonVariant variant = "hello";
    REQUIRE(std::string("hello") == variant.as<const char*>());
  }

  SECTION("RandomStringAsCharPtr") {
    JsonVariant variant = "hello";
    REQUIRE(std::string("hello") == variant.as<char*>());
  }

  SECTION("RandomStringAsString") {
    JsonVariant variant = "hello";
    REQUIRE(std::string("hello") == variant.as<std::string>());
  }

  SECTION("TrueStringAsBool") {
    JsonVariant variant = "true";
    REQUIRE(variant.as<bool>());
  }

  SECTION("TrueStringAsLong") {
    JsonVariant variant = "true";
    REQUIRE(1L == variant.as<long>());
  }

  SECTION("ObjectAsString") {
    DynamicJsonBuffer buffer;

    JsonObject& obj = buffer.createObject();
    obj["key"] = "value";

    JsonVariant variant = obj;
    REQUIRE(std::string("{\"key\":\"value\"}") == variant.as<std::string>());
  }

  SECTION("ArrayAsString") {
    DynamicJsonBuffer buffer;

    JsonArray& arr = buffer.createArray();
    arr.add(4);
    arr.add(2);

    JsonVariant variant = arr;
    REQUIRE(std::string("[4,2]") == variant.as<std::string>());
  }

  SECTION("ArrayAsJsonArray") {
    DynamicJsonBuffer buffer;
    JsonArray& arr = buffer.createArray();

    JsonVariant variant = arr;
    REQUIRE(&arr == &variant.as<JsonArray&>());
    REQUIRE(&arr == &variant.as<JsonArray>());  // <- shorthand
  }

  SECTION("ObjectAsJsonObject") {
    DynamicJsonBuffer buffer;
    JsonObject& arr = buffer.createObject();

    JsonVariant variant = arr;
    REQUIRE(&arr == &variant.as<JsonObject&>());
    REQUIRE(&arr == &variant.as<JsonObject>());  // <- shorthand
  }
}
