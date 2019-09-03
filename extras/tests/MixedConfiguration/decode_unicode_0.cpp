#define ARDUINOJSON_DECODE_UNICODE 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_DECODE_UNICODE == 0") {
  DynamicJsonDocument doc(2048);
  DeserializationError err = deserializeJson(doc, "\"\\uD834\\uDD1E\"");

  REQUIRE(err == DeserializationError::NotSupported);
}
