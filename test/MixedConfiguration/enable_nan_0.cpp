#define ARDUINOJSON_ENABLE_NAN 0
#include <ArduinoJson.h>

#include <catch.hpp>
#include <limits>

TEST_CASE("ARDUINOJSON_ENABLE_NAN == 0") {
  DynamicJsonDocument doc(4096);
  JsonObject root = doc.to<JsonObject>();

  SECTION("serializeJson()") {
    root["X"] = std::numeric_limits<double>::signaling_NaN();

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"X\":null}");
  }

  SECTION("deserializeJson()") {
    auto err = deserializeJson(doc, "{\"X\":NaN}");

    REQUIRE(err == DeserializationError::InvalidInput);
  }
}
