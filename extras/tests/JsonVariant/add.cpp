// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonVariant::add(T)") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("add integer to new variant") {
    var.add(42);

    REQUIRE(var.as<std::string>() == "[42]");
  }

  SECTION("add const char* to new variant") {
    var.add("hello");

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }

  SECTION("add std::string to new variant") {
    var.add(std::string("hello"));

    REQUIRE(var.as<std::string>() == "[\"hello\"]");
  }

  SECTION("add integer to integer") {
    var.set(123);

    var.add(456);  // no-op

    REQUIRE(var.as<std::string>() == "123");
  }

  SECTION("add integer to object") {
    var["val"] = 123;

    var.add(456);  // no-op

    REQUIRE(var.as<std::string>() == "{\"val\":123}");
  }
}

TEST_CASE("JsonVariant::add<T>()") {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();

  SECTION("JsonArray") {
    JsonArray array = var.add<JsonArray>();
    array.add(1);
    array.add(2);
    REQUIRE(doc.as<std::string>() == "[[1,2]]");
  }

  SECTION("JsonObject") {
    JsonObject object = var.add<JsonObject>();
    object["hello"] = "world";
    REQUIRE(doc.as<std::string>() == "[{\"hello\":\"world\"}]");
  }

  SECTION("JsonVariant") {
    JsonVariant variant = var.add<JsonVariant>();
    variant.set(42);
    REQUIRE(doc.as<std::string>() == "[42]");
  }
}
