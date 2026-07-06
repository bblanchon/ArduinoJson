// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

using ArduinoJson::detail::is_base_of;

TEST_CASE("DynamicJsonDocument") {
  SUBCASE("is a JsonDocument") {
    REQUIRE((is_base_of<JsonDocument, DynamicJsonDocument>::value == true));
  }

  SUBCASE("deserialize / serialize") {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, "{\"hello\":\"world\"}");
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SUBCASE("copy") {
    DynamicJsonDocument doc(256);
    doc["hello"] = "world";
    auto copy = doc;
    REQUIRE(copy.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SUBCASE("capacity") {
    DynamicJsonDocument doc(256);
    REQUIRE(doc.capacity() == 256);
  }

  SUBCASE("garbageCollect()") {
    DynamicJsonDocument doc(256);
    doc.garbageCollect();
  }
}
