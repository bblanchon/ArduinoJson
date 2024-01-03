// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using ArduinoJson::detail::is_base_of;

TEST_CASE("StaticJsonDocument") {
  SECTION("is a JsonDocument") {
    REQUIRE(is_base_of<JsonDocument, StaticJsonDocument<256>>::value == true);
  }

  SECTION("deserialize / serialize") {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, "{\"hello\":\"world\"}");
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("copy") {
    StaticJsonDocument<256> doc;
    doc["hello"] = "world";
    auto copy = doc;
    REQUIRE(copy.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("capacity") {
    StaticJsonDocument<256> doc;
    REQUIRE(doc.capacity() == 256);
  }
}
