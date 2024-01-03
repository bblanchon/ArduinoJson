// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("serialize JsonArray to std::string") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  array.add(4);
  array.add(2);

  SECTION("serializeJson()") {
    std::string json = "erase me";
    serializeJson(array, json);

    REQUIRE("[4,2]" == json);
  }

  SECTION("serializeJsonPretty") {
    std::string json = "erase me";
    serializeJsonPretty(array, json);

    REQUIRE("[\r\n  4,\r\n  2\r\n]" == json);
  }
}

TEST_CASE("serialize JsonObject to std::string") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["key"] = "value";

  SECTION("object") {
    std::string json = "erase me";
    serializeJson(doc, json);

    REQUIRE("{\"key\":\"value\"}" == json);
  }

  SECTION("serializeJsonPretty") {
    std::string json = "erase me";
    serializeJsonPretty(doc, json);

    REQUIRE("{\r\n  \"key\": \"value\"\r\n}" == json);
  }
}

TEST_CASE("serialize an std::string containing a NUL") {
  JsonDocument doc;
  doc.set(std::string("hello\0world", 11));

  std::string json = "erase me";
  serializeJson(doc, json);
  CHECK("\"hello\\u0000world\"" == json);
}
