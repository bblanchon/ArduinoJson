// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("serialize JsonArray to std::string") {
  DynamicJsonDocument doc;
  JsonArray &array = doc.to<JsonArray>();
  array.add(4);
  array.add(2);

  SECTION("serializeJson()") {
    std::string json;
    serializeJson(array, json);

    REQUIRE(std::string("[4,2]") == json);
  }

  SECTION("serializeJsonPretty") {
    std::string json;
    serializeJsonPretty(array, json);

    REQUIRE(std::string("[\r\n  4,\r\n  2\r\n]") == json);
  }
}

TEST_CASE("serialize JsonObject to std::string") {
  DynamicJsonDocument doc;
  JsonObject &obj = doc.to<JsonObject>();
  obj["key"] = "value";

  SECTION("object") {
    std::string json;
    serializeJson(doc, json);

    REQUIRE(std::string("{\"key\":\"value\"}") == json);
  }

  SECTION("serializeJsonPretty") {
    std::string json;
    serializeJsonPretty(doc, json);

    REQUIRE(std::string("{\r\n  \"key\": \"value\"\r\n}") == json);
  }
}
