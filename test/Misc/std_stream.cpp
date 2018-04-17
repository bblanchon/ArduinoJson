// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <sstream>

TEST_CASE("std::stream") {
  SECTION("JsonVariantFalse") {
    std::ostringstream os;
    JsonVariant variant = false;
    os << variant;
    REQUIRE("false" == os.str());
  }

  SECTION("JsonVariantString") {
    std::ostringstream os;
    JsonVariant variant = "coucou";
    os << variant;
    REQUIRE("\"coucou\"" == os.str());
  }

  SECTION("JsonObject") {
    std::ostringstream os;
    DynamicJsonDocument doc;
    JsonObject& object = doc.to<JsonObject>();
    object["key"] = "value";
    os << object;
    REQUIRE("{\"key\":\"value\"}" == os.str());
  }

  SECTION("JsonObjectSubscript") {
    std::ostringstream os;
    DynamicJsonDocument doc;
    JsonObject& object = doc.to<JsonObject>();
    object["key"] = "value";
    os << object["key"];
    REQUIRE("\"value\"" == os.str());
  }

  SECTION("JsonArray") {
    std::ostringstream os;
    DynamicJsonDocument doc;
    JsonArray& array = doc.to<JsonArray>();
    array.add("value");
    os << array;
    REQUIRE("[\"value\"]" == os.str());
  }

  SECTION("JsonArraySubscript") {
    std::ostringstream os;
    DynamicJsonDocument doc;
    JsonArray& array = doc.to<JsonArray>();
    array.add("value");
    os << array[0];
    REQUIRE("\"value\"" == os.str());
  }

  SECTION("ParseArray") {
    std::istringstream json(" [ 42 /* comment */ ] ");
    DynamicJsonDocument doc;
    JsonError err = deserializeJson(doc, json);
    JsonArray& arr = doc.as<JsonArray>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(1 == arr.size());
    REQUIRE(42 == arr[0]);
  }

  SECTION("ParseObject") {
    std::istringstream json(" { hello : world // comment\n }");
    DynamicJsonDocument doc;
    JsonError err = deserializeJson(doc, json);
    JsonObject& obj = doc.as<JsonObject>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(1 == obj.size());
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("ShouldNotReadPastTheEnd") {
    std::istringstream json("{}123");
    DynamicJsonDocument doc;
    deserializeJson(doc, json);
    REQUIRE('1' == json.get());
  }
}
