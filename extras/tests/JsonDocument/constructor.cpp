// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::addPadding;

TEST_CASE("JsonDocument constructor") {
  SECTION("Copy constructor") {
    JsonDocument doc1(1234);
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    JsonDocument doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");

    REQUIRE(doc2.capacity() == doc1.capacity());
  }

  SECTION("Construct from JsonObject") {
    JsonDocument doc1(200);
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2 = obj;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }

  SECTION("Construct from JsonArray") {
    JsonDocument doc1(200);
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2 = arr;

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }

  SECTION("Construct from JsonVariant") {
    JsonDocument doc1(200);
    deserializeJson(doc1, "42");

    JsonDocument doc2 = doc1.as<JsonVariant>();

    REQUIRE(doc2.as<std::string>() == "42");
    REQUIRE(doc2.capacity() == addPadding(doc1.memoryUsage()));
  }
}
