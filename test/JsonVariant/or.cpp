// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static const JsonVariant undefined;
static const JsonVariant null = static_cast<const char*>(0);

TEST_CASE("JsonVariant::operator|()") {
  SECTION("undefined | const char*") {
    std::string result = undefined | "default";
    REQUIRE(result == "default");
  }

  SECTION("undefined | int") {
    int result = undefined | 42;
    REQUIRE(result == 42);
  }

  SECTION("undefined | bool") {
    bool result = undefined | true;
    REQUIRE(result == true);
  }

  SECTION("null | const char*") {
    std::string result = null | "default";
    REQUIRE(result == "default");
  }

  SECTION("null | int") {
    int result = null | 42;
    REQUIRE(result == 42);
  }

  SECTION("null | bool") {
    bool result = null | true;
    REQUIRE(result == true);
  }

  SECTION("int | const char*") {
    JsonVariant variant = 42;
    std::string result = variant | "default";
    REQUIRE(result == "default");
  }

  SECTION("int | int") {
    JsonVariant variant = 0;
    int result = variant | 666;
    REQUIRE(result == 0);
  }

  SECTION("double | int") {
    JsonVariant variant = 42.0;
    int result = variant | 666;
    REQUIRE(result == 42);
  }

  SECTION("bool | bool") {
    JsonVariant variant = false;
    bool result = variant | true;
    REQUIRE(result == false);
  }

  SECTION("int | bool") {
    JsonVariant variant = 0;
    bool result = variant | true;
    REQUIRE(result == true);
  }

  SECTION("const char* | const char*") {
    JsonVariant variant = "not default";
    std::string result = variant | "default";
    REQUIRE(result == "not default");
  }

  SECTION("const char* | int") {
    JsonVariant variant = "not default";
    int result = variant | 42;
    REQUIRE(result == 42);
  }
}
