// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("DynamicJsonDocument") {
  DynamicJsonDocument doc;

  SECTION("serializeJson()") {
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = "world";

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"hello\":\"world\"}");
  }

  SECTION("memoryUsage()") {
    SECTION("starts at zero") {
      REQUIRE(doc.memoryUsage() == 0);
    }

    SECTION("JSON_ARRAY_SIZE(0)") {
      doc.to<JsonArray>();
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(0));
    }

    SECTION("JSON_ARRAY_SIZE(1)") {
      doc.to<JsonArray>().add(42);
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1));
    }

    SECTION("JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(0)") {
      doc.to<JsonArray>().createNestedArray();
      REQUIRE(doc.memoryUsage() == JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(0));
    }
  }
}
