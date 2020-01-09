// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::operator[]") {
  DynamicJsonDocument doc(4096);
  const JsonDocument& cdoc = doc;

  SECTION("object") {
    deserializeJson(doc, "{\"hello\":\"world\"}");

    SECTION("const char*") {
      REQUIRE(doc["hello"] == "world");
      REQUIRE(cdoc["hello"] == "world");
    }

    SECTION("std::string") {
      REQUIRE(doc[std::string("hello")] == "world");
      REQUIRE(cdoc[std::string("hello")] == "world");
    }

    SECTION("supports operator|") {
      REQUIRE((doc["hello"] | "nope") == std::string("world"));
      REQUIRE((doc["world"] | "nope") == std::string("nope"));
    }
  }

  SECTION("array") {
    deserializeJson(doc, "[\"hello\",\"world\"]");

    REQUIRE(doc[1] == "world");
    REQUIRE(cdoc[1] == "world");
  }
}

TEST_CASE("JsonDocument automatically promotes to object") {
  DynamicJsonDocument doc(4096);

  doc["one"]["two"]["three"] = 4;

  REQUIRE(doc["one"]["two"]["three"] == 4);
}
