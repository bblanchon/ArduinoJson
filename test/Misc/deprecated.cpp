// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#define ARDUINOJSON_ENABLE_DEPRECATED 1

#include <ArduinoJson.h>
#include <catch.hpp>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

TEST_CASE("Deprecated functions") {
  DynamicJsonBuffer jsonBuffer;

  SECTION("JsonVariant::asArray()") {
    JsonVariant variant = jsonBuffer.createArray();
    REQUIRE(variant.asArray().success());
  }

  SECTION("JsonVariant::asObject()") {
    JsonVariant variant = jsonBuffer.createObject();
    REQUIRE(variant.asObject().success());
  }

  SECTION("JsonVariant::asString()") {
    JsonVariant variant = "hello";
    REQUIRE(std::string("hello") == variant.asString());
  }

  SECTION("JsonArray::removeAt()") {
    JsonArray& arr = jsonBuffer.createArray();
    arr.removeAt(0);
  }

  SECTION("JsonVariant::JsonVariant(float, uint8_t)") {
    JsonVariant variant(3.14f, 2);
    REQUIRE(variant == 3.14f);
  }

  SECTION("JsonVariant::JsonVariant(double, uint8_t)") {
    JsonVariant variant(3.14, 2);
    REQUIRE(variant == 3.14);
  }

  SECTION("float_with_n_digits()") {
    JsonVariant variant = float_with_n_digits(3.14f, 4);
    REQUIRE(variant == 3.14f);
  }

  SECTION("double_with_n_digits()") {
    JsonVariant variant = double_with_n_digits(3.14f, 4);
    REQUIRE(variant == 3.14f);
  }

  SECTION("JsonArraySubscript::set(double, uint8_t)") {
    JsonArray& arr = jsonBuffer.createArray();
    arr.add(666);
    arr[0].set(123.45, 2);
    REQUIRE(123.45 == arr[0].as<double>());
    REQUIRE(true == arr[0].is<double>());
    REQUIRE(false == arr[0].is<int>());
  }

  SECTION("JsonArray::add(double, uint8_t)") {
    JsonArray& arr = jsonBuffer.createArray();
    arr.add(3.14159265358979323846, 4);
  }

  SECTION("JsonArray::add(float, uint8_t)") {
    JsonArray& arr = jsonBuffer.createArray();
    arr.add(3.14159265358979323846f, 4);
  }

  SECTION("JsonObject::set(unsigned char[], double, uint8_t)") {
    unsigned char key[] = "hello";

    JsonObject& obj = jsonBuffer.createObject();
    obj.set(key, 3.14, 2);

    REQUIRE(3.14 == obj["hello"]);
  }

  SECTION("JsonObject::set(const char*, double, uint8_t)") {
    JsonObject& obj = jsonBuffer.createObject();
    obj.set("hello", 123.45, 2);

    REQUIRE(123.45 == obj["hello"].as<double>());
    REQUIRE(obj["hello"].is<double>());
    REQUIRE_FALSE(obj["hello"].is<long>());
  }

  SECTION("JsonObjectSubscript::set(double, uint8_t)") {
    JsonObject& obj = jsonBuffer.createObject();
    obj["hello"].set(123.45, 2);

    REQUIRE(true == obj["hello"].is<double>());
    REQUIRE(false == obj["hello"].is<long>());
    REQUIRE(123.45 == obj["hello"].as<double>());
  }
}
