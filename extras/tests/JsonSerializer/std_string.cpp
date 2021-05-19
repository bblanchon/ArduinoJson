// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("serialize JsonArray to std::string") {
  DynamicJsonDocument doc(4096);
  JsonArray array = doc.to<JsonArray>();
  array.add(4);
  array.add(2);

  SECTION("serializeJson()") {
    std::string json;
    serializeJson(array, json);

    REQUIRE("[4,2]" == json);
  }

  SECTION("serializeJsonPretty") {
    std::string json;
    serializeJsonPretty(array, json);

    REQUIRE("[\r\n  4,\r\n  2\r\n]" == json);
  }
}

TEST_CASE("serialize JsonObject to std::string") {
  DynamicJsonDocument doc(4096);
  JsonObject obj = doc.to<JsonObject>();
  obj["key"] = "value";

  SECTION("object") {
    std::string json;
    serializeJson(doc, json);

    REQUIRE("{\"key\":\"value\"}" == json);
  }

  SECTION("serializeJsonPretty") {
    std::string json;
    serializeJsonPretty(doc, json);

    REQUIRE("{\r\n  \"key\": \"value\"\r\n}" == json);
  }
}
