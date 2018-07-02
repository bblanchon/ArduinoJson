// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <sstream>

TEST_CASE("deserializeJson(std::istream&)") {
  DynamicJsonDocument doc;

  SECTION("array") {
    std::istringstream json(" [ 42 /* comment */ ] ");

    DeserializationError err = deserializeJson(doc, json);
    JsonArray arr = doc.as<JsonArray>();

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(1 == arr.size());
    REQUIRE(42 == arr[0]);
  }

  SECTION("object") {
    std::istringstream json(" { hello : world // comment\n }");

    DeserializationError err = deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(1 == obj.size());
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("Should not read after the closing brace of an empty object") {
    std::istringstream json("{}123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing brace") {
    std::istringstream json("{\"hello\":\"world\"}123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing bracket of an empty array") {
    std::istringstream json("[]123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing bracket") {
    std::istringstream json("[\"hello\",\"world\"]123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing quote") {
    std::istringstream json("\"hello\"123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }
}
