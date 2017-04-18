// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

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
  SECTION("JsonVariant::asArray()") {
    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.createArray();
    REQUIRE(variant.asArray().success());
  }

  SECTION("JsonVariant::asObject()") {
    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.createObject();
    REQUIRE(variant.asObject().success());
  }

  SECTION("JsonVariant::asString()") {
    JsonVariant variant = "hello";
    REQUIRE(std::string("hello") == variant.asString());
  }

  SECTION("JsonArray::removeAt()") {
    DynamicJsonBuffer jsonBuffer;
    JsonArray& arr = jsonBuffer.createArray();
    arr.removeAt(0);
  }
}
