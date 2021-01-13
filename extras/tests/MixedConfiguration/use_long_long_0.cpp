#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 0") {
  DynamicJsonDocument doc(4096);

  doc["A"] = 42;
  doc["B"] = 84;

  std::string json;
  serializeJson(doc, json);

  REQUIRE(json == "{\"A\":42,\"B\":84}");
}
