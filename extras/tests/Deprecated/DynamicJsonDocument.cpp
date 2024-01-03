// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::is_base_of;

TEST_CASE("DynamicJsonDocument") {
  SECTION("is a JsonDocument") {
    REQUIRE(is_base_of<JsonDocument, DynamicJsonDocument>::value == true);
  }

  SECTION("deserialize / serialize") {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, "{\"hello\":\"world\"}");
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("copy") {
    DynamicJsonDocument doc(256);
    doc["hello"] = "world";
    auto copy = doc;
    REQUIRE(copy.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("capacity") {
    DynamicJsonDocument doc(256);
    REQUIRE(doc.capacity() == 256);
  }

  SECTION("garbageCollect()") {
    DynamicJsonDocument doc(256);
    doc.garbageCollect();
  }
}
