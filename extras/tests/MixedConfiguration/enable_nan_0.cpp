#define ARDUINOJSON_ENABLE_NAN 0
#include <ArduinoJson.h>

#include <doctest.h>
#include <limits>

TEST_CASE("ARDUINOJSON_ENABLE_NAN == 0") {
  JsonDocument doc;
  JsonObject root = doc.to<JsonObject>();

  SUBCASE("serializeJson()") {
    root["X"] = std::numeric_limits<double>::signaling_NaN();

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"X\":null}");
  }

  SUBCASE("deserializeJson()") {
    DeserializationError err = deserializeJson(doc, "{\"X\":NaN}");

    REQUIRE(err == DeserializationError::InvalidInput);
  }
}
