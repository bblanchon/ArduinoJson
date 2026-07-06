#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>

#include <doctest.h>

TEST_CASE("ARDUINOJSON_DECODE_UNICODE == 1") {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, "\"\\uD834\\uDD1E\"");

  REQUIRE(err == DeserializationError::Ok);
}
