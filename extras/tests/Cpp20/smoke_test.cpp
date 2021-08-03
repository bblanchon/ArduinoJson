#include <ArduinoJson.h>

#include <catch.hpp>
#include <string>

TEST_CASE("C++20 smoke test") {
  StaticJsonDocument<128> doc;

  deserializeJson(doc, "{\"hello\":\"world\"}");
  REQUIRE(doc["hello"] == "world");

  std::string json;
  serializeJson(doc, json);
  REQUIRE(json == "{\"hello\":\"world\"}");
}
