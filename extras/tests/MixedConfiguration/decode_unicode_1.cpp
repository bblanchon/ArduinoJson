#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_DECODE_UNICODE == 1") {
  DynamicJsonDocument doc(2048);
  DeserializationError err = deserializeJson(doc, "\"\\uD834\\uDD1E\"");

  REQUIRE(err == DeserializationError::Ok);
}
