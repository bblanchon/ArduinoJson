// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::operator|()") {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("undefined") {
    SECTION("undefined | const char*") {
      std::string result = variant | "default";
      REQUIRE(result == "default");
    }

    SECTION("undefined | int") {
      int result = variant | 42;
      REQUIRE(result == 42);
    }

    SECTION("undefined | bool") {
      bool result = variant | true;
      REQUIRE(result == true);
    }
  }

  SECTION("null") {
    variant.set(static_cast<const char*>(0));

    SECTION("null | const char*") {
      std::string result = variant | "default";
      REQUIRE(result == "default");
    }

    SECTION("null | int") {
      int result = variant | 42;
      REQUIRE(result == 42);
    }

    SECTION("null | bool") {
      bool result = variant | true;
      REQUIRE(result == true);
    }
  }

  SECTION("int | const char*") {
    variant.set(42);
    std::string result = variant | "default";
    REQUIRE(result == "default");
  }

  SECTION("int | int") {
    variant.set(0);
    int result = variant | 666;
    REQUIRE(result == 0);
  }

  SECTION("double | int") {
    variant.set(42.0);
    int result = variant | 666;
    REQUIRE(result == 42);
  }

  SECTION("bool | bool") {
    variant.set(false);
    bool result = variant | true;
    REQUIRE(result == false);
  }

  SECTION("int | bool") {
    variant.set(0);
    bool result = variant | true;
    REQUIRE(result == true);
  }

  SECTION("const char* | const char*") {
    variant.set("not default");
    std::string result = variant | "default";
    REQUIRE(result == "not default");
  }

  SECTION("const char* | int") {
    variant.set("not default");
    int result = variant | 42;
    REQUIRE(result == 42);
  }
}
