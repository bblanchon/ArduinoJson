// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson.h>
#include <doctest.h>

using ArduinoJson::detail::is_base_of;

TEST_CASE("StaticJsonDocument") {
  SUBCASE("is a JsonDocument") {
    REQUIRE((is_base_of<JsonDocument, StaticJsonDocument<256>>::value == true));
  }

  SUBCASE("deserialize / serialize") {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, "{\"hello\":\"world\"}");
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SUBCASE("copy") {
    StaticJsonDocument<256> doc;
    doc["hello"] = "world";
    auto copy = doc;
    REQUIRE(copy.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SUBCASE("capacity") {
    StaticJsonDocument<256> doc;
    REQUIRE(doc.capacity() == 256);
  }
}
