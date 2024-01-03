// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonDocument::operator[]") {
  JsonDocument doc;
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
  JsonDocument doc;

  doc["one"]["two"]["three"] = 4;

  REQUIRE(doc["one"]["two"]["three"] == 4);
}

TEST_CASE("JsonDocument automatically promotes to array") {
  JsonDocument doc;

  doc[2] = 2;

  REQUIRE(doc.as<std::string>() == "[null,null,2]");
}
