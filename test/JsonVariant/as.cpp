// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

static const char* null = 0;

TEST_CASE("JsonVariant::as()") {
  DynamicJsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("DoubleAsBool") {
    variant.set(4.2);
    REQUIRE(variant.as<bool>());
  }

  SECTION("DoubleAsCstr") {
    variant.set(4.2);
    REQUIRE_FALSE(variant.as<const char*>());
  }

  SECTION("DoubleAsString") {
    variant.set(4.2);
    REQUIRE(std::string("4.2") == variant.as<std::string>());
  }

  SECTION("DoubleAsLong") {
    variant.set(4.2);
    REQUIRE(4L == variant.as<long>());
  }

  SECTION("DoubleAsUnsigned") {
    variant.set(4.2);
    REQUIRE(4U == variant.as<unsigned>());
  }

  SECTION("DoubleZeroAsBool") {
    variant.set(0.0);
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("DoubleZeroAsLong") {
    variant.set(0.0);
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("FalseAsBool") {
    variant.set(false);
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("FalseAsDouble") {
    variant.set(false);
    REQUIRE(0.0 == variant.as<double>());
  }

  SECTION("FalseAsLong") {
    variant.set(false);
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("FalseAsString") {
    variant.set(false);
    REQUIRE(std::string("false") == variant.as<std::string>());
  }

  SECTION("TrueAsBool") {
    variant.set(true);
    REQUIRE(variant.as<bool>());
  }

  SECTION("TrueAsDouble") {
    variant.set(true);
    REQUIRE(1.0 == variant.as<double>());
  }

  SECTION("TrueAsLong") {
    variant.set(true);
    REQUIRE(1L == variant.as<long>());
  }

  SECTION("TrueAsString") {
    variant.set(true);
    REQUIRE(std::string("true") == variant.as<std::string>());
  }

  SECTION("LongAsBool") {
    variant.set(42L);
    REQUIRE(variant.as<bool>());
  }

  SECTION("LongZeroAsBool") {
    variant.set(0L);
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("PositiveLongAsDouble") {
    variant.set(42L);
    REQUIRE(42.0 == variant.as<double>());
  }

  SECTION("NegativeLongAsDouble") {
    variant.set(-42L);
    REQUIRE(-42.0 == variant.as<double>());
  }

  SECTION("LongAsString") {
    variant.set(42L);
    REQUIRE(std::string("42") == variant.as<std::string>());
  }

  SECTION("LongZeroAsDouble") {
    variant.set(0L);
    REQUIRE(0.0 == variant.as<double>());
  }

  SECTION("NullAsBool") {
    variant.set(null);
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("NullAsDouble") {
    variant.set(null);
    REQUIRE(0.0 == variant.as<double>());
  }

  SECTION("NullAsLong") {
    variant.set(null);
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("NullAsString") {
    variant.set(null);
    REQUIRE(std::string("null") == variant.as<std::string>());
  }

  SECTION("NumberStringAsBool") {
    variant.set("42");
    REQUIRE(variant.as<bool>());
  }

  SECTION("NumberStringAsLong") {
    variant.set("42");
    REQUIRE(42L == variant.as<long>());
  }

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
  SECTION("NumberStringAsInt64Negative") {
    variant.set("-9223372036854775808");
    REQUIRE(-9223372036854775807 - 1 == variant.as<long long>());
  }

  SECTION("NumberStringAsInt64Positive") {
    variant.set("9223372036854775807");
    REQUIRE(9223372036854775807 == variant.as<long long>());
  }
#endif

  SECTION("RandomStringAsBool") {
    variant.set("hello");
    REQUIRE_FALSE(variant.as<bool>());
  }

  SECTION("RandomStringAsLong") {
    variant.set("hello");
    REQUIRE(0L == variant.as<long>());
  }

  SECTION("RandomStringAsConstCharPtr") {
    variant.set("hello");
    REQUIRE(std::string("hello") == variant.as<const char*>());
  }

  SECTION("RandomStringAsCharPtr") {
    variant.set("hello");
    REQUIRE(std::string("hello") == variant.as<char*>());
  }

  SECTION("RandomStringAsString") {
    variant.set("hello");
    REQUIRE(std::string("hello") == variant.as<std::string>());
  }

  SECTION("TrueStringAsBool") {
    variant.set("true");
    REQUIRE(variant.as<bool>());
  }

  SECTION("TrueStringAsLong") {
    variant.set("true");
    REQUIRE(1L == variant.as<long>());
  }

  SECTION("ObjectAsString") {
    DynamicJsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    obj["key"] = "value";

    variant.set(obj);
    REQUIRE(std::string("{\"key\":\"value\"}") == variant.as<std::string>());
  }

  SECTION("ArrayAsString") {
    DynamicJsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();
    arr.add(4);
    arr.add(2);

    variant.set(arr);
    REQUIRE(std::string("[4,2]") == variant.as<std::string>());
  }

  SECTION("ArrayAsJsonArray") {
    DynamicJsonDocument doc2;
    JsonArray arr = doc2.to<JsonArray>();

    variant.set(arr);
    REQUIRE(arr == variant.as<JsonArray>());
    REQUIRE(arr == variant.as<JsonArray>());  // <- shorthand
  }

  SECTION("ObjectAsJsonObject") {
    DynamicJsonDocument doc2;
    JsonObject obj = doc2.to<JsonObject>();

    variant.set(obj);
    REQUIRE(obj == variant.as<JsonObject>());
    REQUIRE(obj == variant.as<JsonObject>());  // <- shorthand
  }
}
