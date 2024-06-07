// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

#include "Literals.hpp"

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
      REQUIRE(doc["hello"_s] == "world");
      REQUIRE(cdoc["hello"_s] == "world");
    }

    SECTION("JsonVariant") {
      doc["key"] = "hello";
      REQUIRE(doc[doc["key"]] == "world");
      REQUIRE(cdoc[cdoc["key"]] == "world");
    }

    SECTION("supports operator|") {
      REQUIRE((doc["hello"] | "nope") == "world"_s);
      REQUIRE((doc["world"] | "nope") == "nope"_s);
    }
  }

  SECTION("array") {
    deserializeJson(doc, "[\"hello\",\"world\"]");

    SECTION("int") {
      REQUIRE(doc[1] == "world");
      REQUIRE(cdoc[1] == "world");
    }

    SECTION("JsonVariant") {
      doc[2] = 1;
      REQUIRE(doc[doc[2]] == "world");
      REQUIRE(cdoc[doc[2]] == "world");
    }
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
