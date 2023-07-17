#include <ArduinoJson.h>
#include <catch.hpp>
#include <limits>

TEST_CASE("serializeJson(MemberProxy)") {
  JsonDocument doc;
  deserializeJson(doc, "{\"hello\":42}");
  JsonObject obj = doc.as<JsonObject>();
  std::string result;

  serializeJson(obj["hello"], result);

  REQUIRE(result == "42");
}

TEST_CASE("serializeJson(ElementProxy)") {
  JsonDocument doc;
  deserializeJson(doc, "[42]");
  JsonArray arr = doc.as<JsonArray>();
  std::string result;

  serializeJson(arr[0], result);

  REQUIRE(result == "42");
}

TEST_CASE("serializeJson(JsonVariantSubscript)") {
  JsonDocument doc;
  deserializeJson(doc, "[42]");
  JsonVariant var = doc.as<JsonVariant>();
  std::string result;

  serializeJson(var[0], result);

  REQUIRE(result == "42");
}
