#define ARDUINOJSON_ENABLE_INFINITY 0
#include <ArduinoJson.h>

#include <doctest.h>
#include <limits>

static void assertParseFails(const char* json) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, json);

  REQUIRE(err == DeserializationError::InvalidInput);
}

static void assertJsonEquals(const JsonDocument& doc,
                             std::string expectedJson) {
  std::string actualJson;
  serializeJson(doc, actualJson);
  REQUIRE(actualJson == expectedJson);
}

TEST_CASE("ARDUINOJSON_ENABLE_INFINITY == 0") {
  SUBCASE("serializeJson()") {
    JsonDocument doc;
    doc.add(std::numeric_limits<double>::infinity());
    doc.add(-std::numeric_limits<double>::infinity());

    assertJsonEquals(doc, "[null,null]");
  }

  SUBCASE("deserializeJson()") {
    assertParseFails("{\"X\":Infinity}");
    assertParseFails("{\"X\":-Infinity}");
    assertParseFails("{\"X\":+Infinity}");
  }
}
